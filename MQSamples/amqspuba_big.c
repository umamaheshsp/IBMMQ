/* @(#) samples/c/amqspuba.c, lqm, p701, p701-109-120718 1.10.1.1 09/08/16 15:29:44 */
 /********************************************************************/
 /*                                                                  */
 /* Program name: AMQSPUBA                                           */
 /*                                                                  */
 /* Description: Sample C program that publishes messages to         */
 /*              a topic (example using MQPUT)                       */
 /* <N_OCO_COPYRIGHT>                                                */
 /* Licensed Materials - Property of IBM                             */
 /*                                                                  */
 /* 5724-H72                                                         */
 /* (c) Copyright IBM Corp. 1994, 2009 All Rights Reserved.          */
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
 /*   AMQSPUBA is a sample C program to publish messages on a topic  */
 /*   and is an example of the use of MQPUT.                         */
 /*                                                                  */
 /*      -- messages are sent to the topic named by the parameter    */
 /*                                                                  */
 /*      -- gets lines from StdIn, and publishes each to target      */
 /*         topic, taking each line of text as the content           */
 /*         of a datagram message; the sample stops when a null      */
 /*         line (or EOF) is read.                                   */
 /*         New-line characters are removed.                         */
 /*         If a line is longer than 99 characters it is broken up   */
 /*         into 99-character pieces. Each piece becomes the         */
 /*         content of a datagram message.                           */
 /*         If the length of a line is a multiple of 99 plus 1       */
 /*         e.g. 199, the last piece will only contain a new-line    */
 /*         character so will terminate the input.                   */
 /*                                                                  */
 /*      -- writes a message for each MQI reason other than          */
 /*         MQRC_NONE; stops if there is a MQI completion code       */
 /*         of MQCC_FAILED                                           */
 /*                                                                  */
 /*    Program logic:                                                */
 /*         MQOPEN topic for OUTPUT                                  */
 /*         while end of input file not reached,                     */
 /*         .  read next line of text                                */
 /*         .  MQPUT datagram message with text line as data         */
 /*         MQCLOSE topic                                            */
 /*                                                                  */
 /*                                                                  */
 /********************************************************************/
 /*                                                                  */
 /*   AMQSPUBA has the following parameters                          */
 /*       required:                                                  */
 /*                 (1) The name of the target topic                 */
 /*       optional:                                                  */
 /*                 (2) Queue manager name                           */
 /*                 (3) The open options                             */
 /*                 (4) The close options                            */
 /*                                                                  */
 /********************************************************************/
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
    /* includes for MQI */
 #include <cmqc.h>
 #include<fcntl.h>
 #include<unistd.h>

 int main(int argc, char **argv)
 {
   /*  Declare file and character for sample input                   */
   FILE *fp;

   /*   Declare MQI structures needed                                */
   MQOD     od = {MQOD_DEFAULT};    /* Object Descriptor             */
   MQMD     md = {MQMD_DEFAULT};    /* Message Descriptor            */
   MQPMO   pmo = {MQPMO_DEFAULT};   /* put message options           */
      /** note, sample uses defaults where it can **/

   MQHCONN  Hcon;                   /* connection handle             */
   MQHOBJ   Hobj;                   /* object handle                 */
   MQLONG   CompCode;               /* completion code               */
   MQLONG   OpenCode;               /* MQOPEN completion code        */
   MQLONG   Reason;                 /* reason code                   */
   MQLONG   CReason;                /* reason code for MQCONN        */
   MQLONG   messlen;                /* message length                */
   char     buffer[100];            /* message buffer                */
   char     mybuf[5242880];
   char     QMName[50];             /* queue manager name            */

   printf("Sample AMQSPUBA start\n");
   if (argc < 2)
   {
     printf("Required parameter missing - topic string\n");
     exit(99);
   }

   /******************************************************************/
   /* Extract arguments                                              */
   /*  argv[1] - Topic String                                        */
   /*  argv[2] - Queue Manager name (optional)                       */
   /*  argv[3] - Publish options (optional)                          */
   /*            e.g. 2097152 = MQPMO_RETAIN - Retain publication    */
   /******************************************************************/
   QMName[0] = 0;    /* default */
   if (argc > 2)
     strncpy(QMName, argv[2], MQ_Q_MGR_NAME_LENGTH);

   if (argc > 3)
   {
     pmo.Options = atoi( argv[3] );
     printf("publish options are %d\n", pmo.Options);
   }
   else
   {
     pmo.Options = MQPMO_FAIL_IF_QUIESCING
                 | MQPMO_NO_SYNCPOINT;
   }

   /******************************************************************/
   /*                                                                */
   /*   Connect to queue manager                                     */
   /*                                                                */
   /******************************************************************/
   MQCONN(QMName,                  /* queue manager                  */
          &Hcon,                   /* connection handle              */
          &CompCode,               /* completion code                */
          &CReason);               /* reason code                    */

   /* report reason and stop if it failed     */
   if (CompCode == MQCC_FAILED)
   {
     printf("MQCONN ended with reason code %d\n", CReason);
     exit( (int)CReason );
   }

   /******************************************************************/
   /*                                                                */
   /*   Use parameter as the name of the target topic                */
   /*                                                                */
   /******************************************************************/
   od.ObjectString.VSPtr=argv[1];
   od.ObjectString.VSLength=(MQLONG)strlen(argv[1]);
   printf("target topic is %s\n", od.ObjectString.VSPtr);

   /******************************************************************/
   /*                                                                */
   /*   Open the target topic for output                             */
   /*                                                                */
   /******************************************************************/
   od.ObjectType = MQOT_TOPIC;
   od.Version = MQOD_VERSION_4;

   MQOPEN(Hcon,                      /* connection handle            */
          &od,                       /* object descriptor for topic  */
          MQOO_OUTPUT | MQOO_FAIL_IF_QUIESCING, /* open options      */
          &Hobj,                     /* object handle                */
          &OpenCode,                 /* MQOPEN completion code       */
          &Reason);                  /* reason code                  */

   /* report reason, if any; stop if failed      */
   if (Reason != MQRC_NONE)
   {
     printf("MQOPEN ended with reason code %d\n", Reason);
   }

   if (OpenCode == MQCC_FAILED)
   {
     printf("unable to open topic for publish\n");
   }

   /******************************************************************/
   /*                                                                */
   /*   Read lines from the file and publish them on the topic       */
   /*   Loop until null line or end of file, or there is a failure   */
   /*                                                                */
   /******************************************************************/
   CompCode = OpenCode;        /* use MQOPEN result for initial test */
   fp = stdin;

   memcpy(md.Format,           /* character string format            */
          MQFMT_STRING, (size_t)MQ_FORMAT_LENGTH);

   while (CompCode != MQCC_FAILED)
   {
     if (fgets(buffer, sizeof(buffer), fp) != NULL)
     {
       messlen = (MQLONG)strlen(buffer); /* length without null      */
       if (buffer[messlen-1] == '\n')  /* last char is a new-line    */
       {
         buffer[messlen-1]  = '\0';    /* replace new-line with null */
         --messlen;                    /* reduce buffer length       */
       {
         int fd;
         fd = open( "/tmp/msg.txt", O_RDONLY );
         messlen = read( fd, mybuf, sizeof(mybuf) );
         mybuf[messlen] = '\0';
       }
       }
     }
     else messlen = 0;        /* treat EOF same as null line         */


     /****************************************************************/
     /*                                                              */
     /*   Publish each buffer to the topic                           */
     /*                                                              */
     /****************************************************************/
     if (messlen > 0)
     {
       MQPUT(Hcon,                /* connection handle               */
             Hobj,                /* object handle                   */
             &md,                 /* message descriptor              */
             &pmo,                /* default options (datagram)      */
             messlen,             /* message length                  */
             mybuf,              /* message buffer                  */
             &CompCode,           /* completion code                 */
             &Reason);            /* reason code                     */

       /* report reason, if any */
       if (Reason != MQRC_NONE)
       {
         printf("MQPUT ended with reason code %d\n", Reason);
       }
     }
     else   /* satisfy end condition when empty line is read */
       CompCode = MQCC_FAILED;
   }

   /******************************************************************/
   /*                                                                */
   /*   Close the target topic (if it was opened)                    */
   /*                                                                */
   /******************************************************************/
   if (OpenCode != MQCC_FAILED)
   {
     MQCLOSE(Hcon,                   /* connection handle            */
             &Hobj,                  /* object handle                */
             MQCO_NONE,
             &CompCode,              /* completion code              */
             &Reason);               /* reason code                  */

     /* report reason, if any     */
     if (Reason != MQRC_NONE)
     {
       printf("MQCLOSE ended with reason code %d\n", Reason);
     }
   }

   /******************************************************************/
   /*                                                                */
   /*   Disconnect from MQM if not already connected                 */
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
   /* END OF AMQSPUBA                                                */
   /*                                                                */
   /******************************************************************/
   printf("Sample AMQSPUBA end\n");
   return(0);
 }
