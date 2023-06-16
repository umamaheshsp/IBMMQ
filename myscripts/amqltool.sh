/^Connection/{end=0; ApplName=""; OrigApplName=""; i1=index($1,"("); i2=index($1,")")-1; ConnId=substr($1,i1+1,i2-i1)}
/ ApplName:/{ApplName=$2}
/ OrigApplName:/{OrigApplName=$2}
/ChannelName:/{ChannelName=$2}
/ShortConnectionName:/{ShortConnectionName=$2; myarray[$2] +=1}
/^}/{ if( end == 0)
      {
        print ConnId " " ApplName " " OrigApplName " " ChannelName " " ShortConnectionName;
      }
    }
/KernelTuningParms/{ end=TRUE; print "Conncount";
for ( x in myarray ) {
	print x "[" myarray[x] "]"
}

                   }

