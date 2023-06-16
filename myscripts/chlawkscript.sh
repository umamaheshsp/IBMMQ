/CHANNEL/{i1=index($1,"("); i2=index($1,")")-1; CHANNEL=substr($1,i1+1,i2-i1);
          i3=index($2,"("); i4=index($2,")")-1; CHLTYPE=substr($2,i3+1,i4-i3);
         }
/MONCHL/{i1=index($1,"("); i2=index($1,")")-1; MONCHL=substr($1,i1+1,i2-i1);
          i3=index($2,"("); i4=index($2,")")-1; MSGS=substr($2,i3+1,i4-i3);
         }
/JOBNAME/{i1=index($1,"("); i2=index($1,")")-1; JOBNAME=substr($1,i1+1,i2-i1);
          i3=index($2,"("); i4=index($2,")")-1; LOCALADDR=substr($2,i3+1,i4-i3);
         }
/BYTSRCVD/{i1=index($1,"("); i2=index($1,")")-1; BYTSRCVD=substr($1,i1+1,i2-i1);
          i3=index($2,"("); i4=index($2,")")-1; BYTSSENT=substr($2,i3+1,i4-i3);
         }
/CONNAME/{CONNAME=$1;
          i3=index($2,"("); i4=index($2,")")-1; CURLUWID=substr($2,i3+1,i4-i3);
         }
/AMQ8417/{

               print  CHANNEL  " " MSGS " " JOBNAME " " CONNAME " " LOCALADDR;
         }

