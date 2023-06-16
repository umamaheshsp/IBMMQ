/* @(#) samples/c/amqscbf0.c, remote, p701, p701-104-110106 1.9.1.1 09/08/15 23:19:50 */
 /********************************************************************/
 /*                                                                  */
 /* Program name: AMQSCBF0                                           */
 /*                                                                  */
 /* Description: Sample C program that gets messages from            */
 /*              a message queue (example using MQCB)                */
 /* <N_OCO_COPYRIGHT>                                                */
 /* Licensed Materials - Property of IBM                             */
 /*                                                                  */
 /* 5724-H72                                                         */
 /* (c) Copyright IBM Corp. 2007, 2009 All Rights Reserved.          */
 /*                                                                  */
 /* US Government Users Restricted Rights - Use, duplication or      */
 /* disclosure restricted by GSA ADP Schedule Contract with          */
 /* IBM Corp.                                                        */
 /* <NOC_COPYRIGHT>                                                  */
 /********************************************************************/
 /*                                                                  */
 /* Function:                                                        */
 /*                                                                  */
 /*                                                                  */
 /*   AMQSCBF0 is a sample C program to get messages from a          */
 /*   message queue, and is an example of MQCB.                      */
 /*                                                                  */
 /*      -- sample reads from message queue named in the parameter   */
 /*                                                                  */
 /*      -- displays the contents of the message queue,              */
 /*         assuming each message data to represent a line of        */
 /*         text to be written                                       */
 /*                                                                  */
 /*         messages are removed from the queue                      */
 /*                                                                  */
 /*      -- writes a message for each MQI reason other than          */
 /*         MQRC_NONE; stops if there is a MQI completion code       */
 /*         of MQCC_FAILED                                           */
 /*                                                                  */
 /*                                                                  */
 /*   Program logic:                                                 */
 /*      Take name of input queue from the parameter                 */
 /*      MQOPEN queue for INPUT                                      */
 /*      MQCB   register a callback function to receive messages     */
 /*      MQCTL  start consumption of messages                        */
 /*      wait for use to press enter                                 */
 /*      MQCTL  stop consumptions of messages                        */
 /*      MQCLOSE the subject queue                                   */
 /*      MQDISC  disconnect from queue manager                       */
 /*                                                                  */
 /*                                                                  */
 /********************************************************************/
 /*                                                                  */
 /*   AMQSCBF0 has the following parameters                          */
 /*       required:                                                  */
 /*                 (1) The name of the source queue or queues       */
 /*       optional:                                                  */
 /*                 (2) -m <name>     Queue manager name             */
 /*                 (3) -o <options>  The open options               */
 /*                                                                  */
 /********************************************************************/
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
                                                /* includes for MQI  */
 #include <cmqc.h>
                                                /* Constants         */
 #define MAX_QUEUES  10
                                                /* Statics           */
 static int Parm_Index = 1;
                                                /* Prototypes        */

 int getparm(int       argc,
             char   ** argv,
             char   ** pFlag,
             char   ** pParm);

   MQHCONN  Hcon = MQHC_UNUSABLE_HCONN;       /* connection handle   */
   MQHOBJ   Hobj = MQHO_UNUSABLE_HOBJ;        /* object handle       */
 void Usage();

 /********************************************************************/
 /* FUNCTION: MessageConsumer                                        */
 /* PURPOSE : Callback function called when messages arrive          */
 /********************************************************************/
 void MessageConsumer(MQHCONN   hConn,
                      MQMD    * pMsgDesc,
                      MQGMO   * pGetMsgOpts,
                      MQBYTE  * Buffer,
                      MQCBC   * pContext)
 {
   MQCNO   cno = {MQCNO_DEFAULT};   /* Connect Options               */
   MQOD     od = {MQOD_DEFAULT};    /* Object Descriptor             */
   MQMD     md = {MQMD_DEFAULT};    /* Message Descriptor            */
   MQGMO   gmo = {MQGMO_DEFAULT};   /* get message options           */
   MQCBD   cbd = {MQCBD_DEFAULT};   /* Callback Descriptor           */
   MQCTLO  ctlo= {MQCTLO_DEFAULT};  /* Control Options               */
   MQLONG   CompCode;               /* completion code               */
   MQLONG   Reason = 999;           /* reason code                   */


   MQLONG i,max;
   MQLONG Length;

   switch(pContext->CallType)
   {
     case MQCBCT_MSG_REMOVED:
     case MQCBCT_MSG_NOT_REMOVED:
          Length = pGetMsgOpts -> ReturnedLength;
          if (pContext->Reason)
            printf("Message Call (%d Bytes) : Reason = %d\n",
                   Length,pContext->Reason);
          else
            printf("Message Call (%d Bytes) :\n",
                   Length);

          /***********************************************************/
          /* Print out only the first printable bytes                */
          /***********************************************************/
          max = Length;
          if (max > 200) max = 200;
          for (i=0; i<max; i++)
          {
            if (isprint(Buffer[i])) fputc(Buffer[i],stdout);
                               else fputc('.',stdout);
          }
          fputc('\n',stdout);
          if (max < Length)
            printf("......plus %d bytes.\n",Length-max);

        MQCB(Hcon,
                MQOP_SUSPEND,
                &cbd,
                Hobj,
                &md,
                &gmo,
                &CompCode,
                &Reason);
     //     printf("MQCB Suspend\n");
          if (CompCode !=  MQCC_OK)
          {
            printf("MQCB ended with reason code %d\n", Reason);
          }

          break;

     case MQCBCT_EVENT_CALL:
          printf("Event Call : Reason = %d\n",pContext->Reason);
          break;

   default:
          printf("Calltype = %d\n",pContext->CallType);
          break;
   }
 }

 /********************************************************************/
 /* FUNCTION: main                                                   */
 /* PURPOSE : Main program entry point                               */
 /********************************************************************/
 int main(int argc, char **argv)
 {

   /*   Declare MQI structures needed                                */
   MQCNO   cno = {MQCNO_DEFAULT};   /* Connect Options               */
   MQOD     od = {MQOD_DEFAULT};    /* Object Descriptor             */
   MQOD     ObjDesc = {MQOD_DEFAULT};    /* Object Descriptor             */
   MQMD     md = {MQMD_DEFAULT};    /* Message Descriptor            */
   MQGMO   gmo = {MQGMO_DEFAULT};   /* get message options           */
   MQCBD   cbd = {MQCBD_DEFAULT};   /* Callback Descriptor           */
   MQCTLO  ctlo= {MQCTLO_DEFAULT};  /* Control Options               */
      /** note, sample uses defaults where it can **/

   MQHCONN  Hconn = MQHC_UNUSABLE_HCONN;       /* connection handle   */
   MQHOBJ   Hobject = MQHO_UNUSABLE_HOBJ;        /* object handle       */
   MQLONG   O_options;              /* MQOPEN options                */
   MQLONG   OpenOptions;              /* MQOPEN options                */
   MQLONG   CompCode;               /* completion code               */
   MQLONG   OpenCode;               /* MQOPEN completion code        */
   MQLONG   Reason = 999;           /* reason code                   */
   MQLONG   CReason;                /* reason code for MQCONN        */
   char     QMName[50] = "";        /* queue manager name            */
   MQCHAR48 Queues[MAX_QUEUES];     /* queue array                   */
   int      Qindex = 0;
   char   * pFlag,* pParm;

   MQMD    MsgDesc = { MQMD_DEFAULT };
   PMQMD   pmdin ;
   MQGMO   GetMsgOpts = { MQGMO_DEFAULT };
   PMQGMO  pgmoin;
   char Buffer[1048576];
   MQLONG  BufferLength = 1048575;
   MQLONG  DataLength;

   printf("Sample AMQSCBF0 start\n\n");

   OpenOptions = MQOO_BROWSE; /* open queue for input      */

   GetMsgOpts.Options = MQGMO_WAIT ;
   GetMsgOpts.Options += MQGMO_BROWSE_NEXT ;
   GetMsgOpts.WaitInterval = 15000; 

   O_options = MQOO_INPUT_AS_Q_DEF      /* open queue for input      */
             | MQOO_FAIL_IF_QUIESCING;  /* but not if MQM stopping   */


   /******************************************************************/
   /* Parse the parameters                                           */
   /******************************************************************/
   while (getparm(argc,argv,&pFlag,&pParm))
   {
     if (pFlag && *pFlag)
     {
                                       /* What have we been passed ? */
       if (!pParm)
       {
         Usage();
         goto MOD_EXIT;
       }
                                       /* All flags must have parm   */
       switch(*pFlag)
       {
                                       /* Queue Manager Name         */
         case 'm':
              strncpy(QMName, pParm, MQ_Q_MGR_NAME_LENGTH);
              break;
                                       /* Open options               */
         case 'o':
              if (!pParm)
              {
                Usage();
                goto MOD_EXIT;
              }
              O_options = atoi( pParm );
              break;
                                       /* Reconnect Options          */
         case 'r':
              switch (*pParm)
              {
                case 'd':
                     cno.Options |= MQCNO_RECONNECT_DISABLED;
                     break;

                case 'm':
                     cno.Options |= MQCNO_RECONNECT_Q_MGR;
                     break;

                case 'r':
                     cno.Options |= MQCNO_RECONNECT;
                     break;

                default:
                     Usage();
                     goto MOD_EXIT;
                     break;
              }
              break;

         default:
              Usage();
              goto MOD_EXIT;
       }
     }
     else
     {
       /**************************************************************/
       /* No flag given.....must be a queue name                     */
       /**************************************************************/
       if (Qindex == MAX_QUEUES)
       {
         printf("Sorry, only a maximum of %d queues is supported\n",
                MAX_QUEUES);
         goto MOD_EXIT;
       }

       if (!pParm)

       {
         Usage();
         goto MOD_EXIT;
       }
       strncpy((char *)&Queues[Qindex++], pParm, MQ_Q_NAME_LENGTH);
     }
   }
   /******************************************************************/
   /* Must get at least one queue name                               */
   /******************************************************************/
   if (Qindex == 0)
   {
     Usage();
     goto MOD_EXIT;
   }
   /******************************************************************/
   /*                                                                */
   /*   Connect to queue manager                                     */
   /*                                                                */
   /******************************************************************/
   MQCONNX(QMName,                 /* queue manager                  */
           &cno,                   /* connect options                */
           &Hcon,                  /* connection handle              */
           &CompCode,              /* completion code                */
           &CReason);              /* reason code                    */

                          /* report reason and stop if it failed     */
   if (CompCode == MQCC_FAILED)
   {
     printf("MQCONN ended with reason code %d\n", CReason);
     exit( (int)CReason );
   }
   Hconn = Hcon;
   /******************************************************************/
   /*                                                                */
   /*   Loop round and open and register the consumers               */
   /*                                                                */
   /******************************************************************/

/*   strncpy(ObjDesc.ObjectName, (char *) &Queues[0], MQ_Q_NAME_LENGTH); */
   strncpy(ObjDesc.ObjectName, "TESTQ", MQ_Q_NAME_LENGTH); 

   while (Qindex--)
   {
     strcpy(od.ObjectName, (char *)&Queues[Qindex]);
     /****************************************************************/
     /*                                                              */
     /*   Open the queue                                             */
     /*                                                              */
     /****************************************************************/
     MQOPEN(Hcon,                    /* connection handle            */
            &od,                     /* object descriptor for queue  */
            O_options,               /* open options                 */
            &Hobj,                   /* object handle                */
            &OpenCode,               /* completion code              */
            &Reason);                /* reason code                  */

     if (OpenCode == MQCC_FAILED)
     {
       printf("MQOPEN of '%.48s' ended with reason code %d\n",
              &Queues[Qindex],Reason);
       goto MOD_EXIT;
     }
     /****************************************************************/
     /*                                                              */
     /*   Register a consumer                                        */
     /*                                                              */
     /****************************************************************/
     cbd.CallbackFunction = MessageConsumer;

     MQCB(Hcon,
          MQOP_REGISTER,
          &cbd,
          Hobj,
          &md,
          &gmo,
          &CompCode,
          &Reason);
     if (CompCode == MQCC_FAILED)
     {
       printf("MQCB ended with reason code %d\n", Reason);
       goto MOD_EXIT;
     }
   }
   /******************************************************************/
   /*                                                                */
   /*  Start consumption of messages                                 */
   /*                                                                */
   /******************************************************************/
   MQCTL(Hcon,
         MQOP_START,
         &ctlo,
         &CompCode,
         &Reason);
   if (CompCode == MQCC_FAILED)
   {
     printf("MQCTL ended with reason code %d\n", Reason);
     goto MOD_EXIT;
   }
   /******************************************************************/
   /*                                                                */
   /*  Wait for the user to press enter                              */
   /*                                                                */
   /******************************************************************/
   {
   /*  char Buffer[10];
     printf("Press enter to end\n");
     fgets(Buffer,sizeof(Buffer),stdin); */
   }

   MQCTL(Hcon,
         MQOP_SUSPEND,
         &ctlo,
         &CompCode,
         &Reason);
   if (CompCode == MQCC_FAILED)
   {
     printf("MQCTL (SUSPEND) ended with reason code %d\n", Reason);
     goto MOD_EXIT;
   }


   sleep(10);
   MQOPEN(Hconn,
          &ObjDesc,
          OpenOptions,
          &Hobject,
          &CompCode,
          &Reason);

   if (CompCode == MQCC_FAILED)
   {
     printf("2nd MQOPEN ended with reason code %d\n", Reason);
     goto MOD_EXIT;
   }


     MQGET(Hconn,
           Hobject,
     /*      pmdin,
           pgmoin, */
           &MsgDesc,
           &GetMsgOpts,
           BufferLength,
           Buffer,
           &DataLength,
           &CompCode,
           &Reason);

     if  (CompCode != MQCC_OK)
     {
       if (Reason != MQRC_NO_MSG_AVAILABLE)
       {
         printf("\n MQGET failed with CompCode:%d Reason:%d",
                CompCode,Reason);
       }
       else
       {
         printf("\n \n \n No more messages ");
         CompCode = MQCC_OK;
         Reason   = MQRC_NONE;
       }
     }
     else
     {
         printf("\n MQGET successful\n");
     }

   /******************************************************************/
   /*                                                                */
   /*  Stop consumption of messages                                  */
   /*                                                                */
   /******************************************************************/
   MQCTL(Hcon,
         MQOP_STOP,
         &ctlo,
         &CompCode,
         &Reason);
   if (CompCode == MQCC_FAILED)
   {
     printf("MQCTL ended with reason code %d\n", Reason);
     goto MOD_EXIT;
   }

MOD_EXIT:
   /******************************************************************/
   /*                                                                */
   /*   Disconnect from MQM if not already connected                 */
   /*                                                                */
   /******************************************************************/
   if (Hcon != MQHC_UNUSABLE_HCONN)
   {
     if (CReason != MQRC_ALREADY_CONNECTED )
     {
       MQDISC(&Hcon,                   /* connection handle          */
              &CompCode,               /* completion code            */
              &Reason);                /* reason code                */

       /* report reason, if any     */
       if (Reason != MQRC_NONE)
       {
         printf("MQDISC ended with reason code %d\n", Reason);
       }
     }
   }
   /******************************************************************/
   /*                                                                */
   /* END OF AMQSCBF0                                                */
   /*                                                                */
   /******************************************************************/
   printf("\nSample AMQSCBF0 end\n");
   return((int)Reason);
 }

 /********************************************************************/
 /* FUNCTION: Usage                                                  */
 /* PURPOSE : Print out the usage for the program                    */
 /********************************************************************/
 void Usage()
 {
   printf("Usage: [Options] <Queue Name> { <Queue Name> }\n");
   printf("  where Options are:\n");
   printf("    -m <Queue Manager Name>\n");
   printf("    -o <Open options>\n");
   printf("    -r <Reconnect Type>\n");
   printf("       d Reconnect Disabled\n");
   printf("       r Reconnect\n");
   printf("       m Reconnect Queue Manager\n");

 }
 /********************************************************************/
 /* FUNCTION: getparm                                                */
 /* PURPOSE : Return parameters from the command line                */
 /********************************************************************/
 int getparm(int       argc,
             char   ** argv,
             char   ** pFlag,
             char   ** pParm)
 {
   int    found = 0;
   char * p     = NULL;
   *pFlag = *pParm = NULL;
   if (Parm_Index >= argc) goto MOD_EXIT;
   found = 1;
   p = argv[Parm_Index++];
   if (*p == '-')
   {
     *pFlag = ++p;                    /* This is a flagged parm      */
     if (!**pFlag) goto MOD_EXIT;     /* No actual flag specified    */
     p++;                             /* Advance to actual parameter */
     if (!*p)                         /* Is it there ?               */
     {
       if (Parm_Index >= argc) goto MOD_EXIT;
       if (*argv[Parm_Index] != '-') *pParm = argv[Parm_Index++];
     }
     else *pParm = p;
   }
   else *pParm = p;
 MOD_EXIT:
   return found;
 }
