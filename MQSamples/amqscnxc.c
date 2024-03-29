/* @(#) samples/c/amqscnxc.c, samples, p701, p701-105-110415 1.18.3.1 09/08/16 20:01:48 */
 /********************************************************************/
 /*                                                                  */
 /* Program name: AMQSCNXC                                           */
 /*                                                                  */
 /* Description: Sample C program that demonstrates how to specify   */
 /*              client connection information on MQCONNX.           */
 /* <N_OCO_COPYRIGHT>                                                */
 /* Licensed Materials - Property of IBM                             */
 /*                                                                  */
 /* 63H9336                                                          */
 /* (c) Copyright IBM Corp. 1999, 2005 All Rights Reserved.          */
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
 /*   AMQSCNXC is a sample C program that demonstrates how to use    */
 /*   the MQCNO structure to supply client connection information    */
 /*   on the MQCONNX call. This enables a client MQI application     */
 /*   to provide the definition of its client connection channel     */
 /*   at run-time without using a client channel table or the        */
 /*   MQSERVER environment variable.                                 */
 /*                                                                  */
 /*   If a connection name, and optionally a server connection       */
 /*   channel name, are supplied, the program constructs a client    */
 /*   connection channel definition in an MQCD structure.            */
 /*                                                                  */
 /*   It then connects to the queue manager using MQCONNX. The       */
 /*   program inquires and prints out the name of the queue manager  */
 /*   to which it connected.                                         */
 /*                                                                  */
 /*   The initial value for the protocol type as assigned using      */
 /*   MQCD_CLIENT_CONN_DEFAULT is MQXPT_TCP. If you want to          */
 /*   connect using a protocol other than TCP/IP, you have to        */
 /*   set ClientConn.TransportType to the appropriate MQXPT_*        */
 /*   value.                                                         */
 /*                                                                  */
 /*   This program is intended to be linked as an MQI client         */
 /*   application. However, it may be linked as a regular MQI        */
 /*   application. Then, it simply connects to a local queue         */
 /*   manager and ignores the client connection information.         */
 /*                                                                  */
 /*                                                                  */
 /********************************************************************/
 /*                                                                  */
 /*   AMQSCNXC has 3 parameters, all of which are optional.          */
 /*                                                                  */
 /*   The usage string is:                                           */
 /*                                                                  */
 /*     amqscnxc [-x ConnName [-c SvrconnChannelName]] [QMgrName]    */
 /*                                                                  */
 /*                                                                  */
 /*   The parameters are:                                            */
 /*                                                                  */
 /*     ConnName    - the connection name of the server queue        */
 /*                   manager in the same format as the CONNAME      */
 /*                   parameter on the MQSC DEFINE CHANNEL command.  */
 /*                                                                  */
 /*                   If this parameter is omitted, the sample       */
 /*                   program will not supply a client               */
 /*                   connection channel definition on the           */
 /*                   MQCONNX call. In this case, it reverts to      */
 /*                   normal MQI client behaviour using a client     */
 /*                   channel table or the MQSERVER environment      */
 /*                   variable to obtain the connection information. */
 /*                                                                  */
 /*     SvrconnChannelName                                           */
 /*                 - the name of the server connection channel      */
 /*                   on the server queue manager with which the     */
 /*                   sample program will try to connect.            */
 /*                                                                  */
 /*                   This parameter may only be specified if        */
 /*                   ConnName is also specified.                    */
 /*                                                                  */
 /*                   If omitted, the default server connection      */
 /*                   channel, SYSTEM.DEF.SVRCONN, is used.          */
 /*                                                                  */
 /*     QMgrName    - the name of the server queue manager.          */
 /*                                                                  */
 /*                   If specified, this parameter must be the last  */
 /*                   parameter on the command line.                 */
 /*                                                                  */
 /*                   If omitted, the sample program will use a      */
 /*                   blank queue manager name.                      */
 /*                                                                  */
 /********************************************************************/
 #include <stdio.h>
 #include <stdlib.h>
 #include <ctype.h>
 #include <string.h>

 /* includes for WebSphere MQ */
 #include <cmqc.h>                  /* For regular MQI definitions   */
 #include <cmqxc.h>                 /* For MQCD definition           */

 #define OK 0
 #define FAIL 1


 /* function prototypes of local functions */
 static int ProcessCommandLine(int argc, char **argv, char **pQMgrName,
                               char **pConnName, char **pChannelName);

 static int GetStringArgument(int argc, char **argv,
                              int *pThisArg,
                              char **pString);


 int myfun1(void *ptr)
 {

   char cspuserid[12];
   /*   Declare MQI structures needed                                */
   MQCNO    Connect_options = {MQCNO_DEFAULT};
                                    /* MQCONNX options               */
   MQCD     ClientConn = {MQCD_CLIENT_CONN_DEFAULT};
                                    /* Client connection channel     */
                                    /* definition                    */
   MQCSP mqcsp = {MQCSP_DEFAULT};
   MQOD     od = {MQOD_DEFAULT};    /* Object Descriptor             */
      /** note, sample uses defaults where it can **/

   char    *p_argQMgrName = NULL;   /* q manager name from user      */
   char    *p_argConnName = NULL;   /* connection name from user     */
   char    *p_argChannelName = NULL;/* channel name from user        */

   MQCHAR   QMName[MQ_Q_MGR_NAME_LENGTH];
                                    /* name of connection q manager  */
   MQHCONN  Hcon;                   /* connection handle             */
   MQHOBJ   Hobj;                   /* object handle                 */
   MQLONG   Selector;               /* selector for inquiry          */
   MQLONG   CompCode;               /* completion code               */
   MQLONG   OpenCode;               /* MQOPEN completion code        */
   MQLONG   Reason;                 /* reason code                   */
   MQLONG   CReason;                /* reason code for MQCONNX       */
   char chlname[24];

   printf("Sample AMQSCNXC start\n");

     /*strncpy(QMName, "MQ71QMGR", MQ_Q_MGR_NAME_LENGTH);*/
     strncpy(QMName, "TQMGR", MQ_Q_MGR_NAME_LENGTH);
     printf("Connecting to queue manager %-48.48s\n", QMName);
     ClientConn.Version = 8;   
     ClientConn.ExitNameLength = 0;
     printf("Exitnamelen[%d]\n", ClientConn.ExitNameLength);

   /******************************************************************/
   /*                                                                */
   /*   Initialise the client channel definition if required         */
   /*                                                                */
   /******************************************************************/
     strncpy(ClientConn.ConnectionName,
             /*"9.121.222.11(14444)",*/
             "9.121.218.39(14567)",
             MQ_CONN_NAME_LENGTH);

     strcpy(chlname, (char *) ptr);
     strncpy(ClientConn.ChannelName,
             chlname,
             MQ_CHANNEL_NAME_LENGTH);

     /* Point the MQCNO to the client connection definition */

     strcpy(ClientConn.RemoteUserIdentifier, "testuser");
     printf("RemoteUserIdentifier [%s]\n", ClientConn.RemoteUserIdentifier);

     strcpy(cspuserid, "idpmqm");
     mqcsp.CSPUserIdPtr = cspuserid;
     mqcsp.CSPUserIdLength = strlen(cspuserid);
     mqcsp.AuthenticationType = MQCSP_AUTH_USER_ID_AND_PWD;
     Connect_options.ClientConnPtr = &ClientConn;
     Connect_options.SecurityParmsPtr = &mqcsp;

     /* Client connection fields are in the version 2 part of the
        MQCNO so we must set the version number to 2 or they will
        be ignored */
     Connect_options.Version = MQCNO_VERSION_5;

     printf("using the server connection channel %s\n",
            ClientConn.ChannelName);
     printf("on connection name %s.\n", ClientConn.ConnectionName);

   /******************************************************************/
   /*                                                                */
   /*   Connect to queue manager                                     */
   /*                                                                */
   /******************************************************************/
   MQCONNX(QMName,                 /* queue manager                  */
           &Connect_options,       /* options for connection         */
           &Hcon,                  /* connection handle              */
           &CompCode,              /* completion code                */
           &CReason);              /* reason code                    */

   /* report reason and stop if it failed     */
   if (CompCode == MQCC_FAILED)
   {
     printf("MQCONNX ended with reason code %d\n", CReason);
     return( (int)CReason );
   }

   /******************************************************************/
   /*                                                                */
   /*   Open the queue manager object to find out its name           */
   /*                                                                */
   /******************************************************************/
   od.ObjectType = MQOT_Q_MGR;       /* open the queue manager object*/
   MQOPEN(Hcon,                      /* connection handle            */
          &od,                       /* object descriptor for queue  */
          MQOO_INQUIRE +             /* open it for inquire          */
            MQOO_FAIL_IF_QUIESCING,  /* but not if MQM stopping      */
          &Hobj,                     /* object handle                */
          &OpenCode,                 /* MQOPEN completion code       */
          &Reason);                  /* reason code                  */

   /* report reason, if any      */
   if (Reason != MQRC_NONE)
   {
     printf("MQOPEN ended with reason code %d\n", Reason);
   }

   if (OpenCode == MQCC_FAILED)
   {
     printf("Unable to open queue manager for inquire\n");
   }

   /******************************************************************/
   /*                                                                */
   /*   Inquire the name of the queue manager                        */
   /*                                                                */
   /******************************************************************/
   if (OpenCode != MQCC_FAILED)
   {
     Selector = MQCA_Q_MGR_NAME;

     MQINQ(Hcon,                     /* connection handle            */
           Hobj,                     /* object handle for q manager  */
           1,                        /* inquire only one selector    */
           &Selector,                /* the selector to inquire      */
           0,                        /* no integer attributes needed */
           NULL,                     /* so no buffer supplied        */
           MQ_Q_MGR_NAME_LENGTH,     /* inquiring a q manager name   */
           QMName,                   /* the buffer for the name      */
           &CompCode,                /* MQINQ completion code        */
           &Reason);                 /* reason code                  */

     if (Reason == MQRC_NONE)
     {
       printf("Connection established to queue manager %-48.48s\n",
              QMName);
     }
     else
     {
       /* report reason, if any */
       printf("MQINQ ended with reason code %d\n", Reason);
     }
   }

   sleep(120);

   /******************************************************************/
   /*                                                                */
   /*   Close the queue manager object (if it was opened)            */
   /*                                                                */
   /******************************************************************/
   if (OpenCode != MQCC_FAILED)
   {
     MQCLOSE(Hcon,                    /* connection handle           */
             &Hobj,                   /* object handle               */
             MQCO_NONE,               /* no close options            */
             &CompCode,               /* completion code             */
             &Reason);                /* reason code                 */

     /* report reason, if any     */
     if (Reason != MQRC_NONE)
     {
       printf("MQCLOSE ended with reason code %d\n", Reason);
     }
   }

   /******************************************************************/
   /*                                                                */
   /*   Disconnect from MQM if not already connected                 */
   /*     - on OS/400, the calling job may be connected before the   */
   /*       sample was run and then it should not disconnect         */
   /*                                                                */
   /******************************************************************/
   if (CReason != MQRC_ALREADY_CONNECTED)
   {
     MQDISC(&Hcon,                   /* connection handle            */
            &CompCode,               /* completion code              */
            &Reason);                /* reason code                  */

     /* report reason, if any     */
     if (Reason != MQRC_NONE)
     {
       printf("MQDISC ended with reason code %d\n", Reason);
     }
   }

   /******************************************************************/
   /*                                                                */
   /* END OF AMQSCNXC                                                */
   /*                                                                */
   /******************************************************************/
   printf("Sample AMQSCNXC end\n");
   return(0);
 }


 /********************************************************************/
 /* This unnecessarily complicated function handles the command line */
 /* arguments. It uses similar rules as the real WebSphere MQ        */
 /* commands for the sake of consistency.                            */
 /********************************************************************/
 static int ProcessCommandLine(int argc, char **argv, char **pQMgrName,
                               char **pConnName, char **pChannelName)
 {
   int      c;                      /* current character             */
   int      argno = 1;              /* Number of current argument    */
   int      rc = OK;                /* return code - OK or FAIL      */

   /******************************************************************/
   /* A single parameter of a question mark gets the usage message   */
   /******************************************************************/
   if ((argc > 1) && (strcmp(argv[1], "?") == 0))
   {
     rc = FAIL;
   }

   /******************************************************************/
   /* Now get whatever we were given from the command line           */
   /******************************************************************/
   while ((rc == OK) && (argno < argc))
   {
     /****************************************************************/
     /* We are at the start of an argument, check for the flag       */
     /* introduction character                                       */
     /****************************************************************/
     if ((argv[argno][0] == '-') || (argv[argno][0] == '/'))
     {
       /**************************************************************/
       /* Get the flag, folding to lower case                        */
       /**************************************************************/
       c = tolower(argv[argno][1]);

       if (c == 'x')                              /* Connection name */
       {
         rc = GetStringArgument(argc, argv, &argno, pConnName);
       }
       else if (c == 'c')                            /* Channel name */
       {
         rc = GetStringArgument(argc, argv, &argno, pChannelName);
       }
       else
       {
         rc = FAIL;
       }
     }
     else
     {
       /**************************************************************/
       /* Have reached end of flag parameters                        */
       /**************************************************************/
       break;
     }
   }

   /******************************************************************/
   /* Check the dependencies between arguments                       */
   /******************************************************************/
   if ((rc == OK) && ((*pConnName == NULL) && (*pChannelName != NULL)))
   {
     rc = FAIL;
   }

   /******************************************************************/
   /* Get the queue manager name                                     */
   /******************************************************************/
   if (rc == OK)
   {
     if (argno == argc - 1)
     {
       *pQMgrName = argv[argno];
     }
     else if (argno != argc)
     {
       /* Unused, dangling arguments */
       rc = FAIL;
     }
   }

   return rc;
 }


 /********************************************************************/
 /* Get the next string argument. It may follow the flag character   */
 /* in the current argument or be in the next argument without a     */
 /* flag.                                                            */
 /********************************************************************/
 static int GetStringArgument(int argc, char **argv,
                              int *pThisArg,
                              char **pString)
 {
   char    *pchCurrent;             /* Ptr to current character      */
   int      argpos = 2;             /* Position within argument      */
   int      rc = OK;                /* Return code                   */

   /******************************************************************/
   /* Set up local variables                                         */
   /******************************************************************/
   pchCurrent = &(argv[*pThisArg][argpos]);


   /******************************************************************/
   /* If we are at the end of an argument...                         */
   /******************************************************************/
   if (*pchCurrent == '\0')
   {
     /****************************************************************/
     /* ... move on to the next one, if there is one                 */
     /****************************************************************/
     if (*pThisArg < argc - 1)
     {
       (*pThisArg)++;
       argpos = 0;
       pchCurrent = argv[*pThisArg];
     }
     else
     {
       rc = FAIL;
     }
   }

   /******************************************************************/
   /* Get the string from the current position but catch strings     */
   /* starting with the flag introduction characters                 */
   /******************************************************************/
   if (rc == OK)
   {
     if ((argpos == 0) &&
         ((*pchCurrent == '-') || (*pchCurrent == '/')))
     {
       rc = FAIL;
     }
     else
     {
       /**************************************************************/
       /* Since the string argument ends at the end of its argument  */
       /* in the argument vector, move on to the next one            */
       /**************************************************************/
       *pString = pchCurrent;
       (*pThisArg)++;
     }
   }

   return rc;
 }

int main( int argc, char *argv[] )
{
  pthread_t tid[10];
  char chl1[24];
  char chl2[24];

/*  strcpy(chl1, "MYCHL");
  strcpy(chl2, "MYCHL");*/
  strcpy(chl1, "SYSTEM.DEF.SVRCONN");
  strcpy(chl2, "SYSTEM.DEF.SVRCONN");
  pthread_create( tid, NULL, myfun1, (void *) chl1 );
  pthread_create( tid+1, NULL, myfun1, (void *) chl2 );


  pthread_join( tid[0], NULL );
  pthread_join( tid[1], NULL );


  return 0;

}

