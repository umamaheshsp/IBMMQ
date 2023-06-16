endmqm SENDER
dltmqm SENDER
crtmqm SENDER
strmqm SENDER
echo "define ql(RECEIVER) usage(XMITQ)
      define listener(MYLSR) TRPTYPE(TCP) PORT(15551) CONTROL(QMGR)
      start listener(MYLSR)
      define channel(SENDER.TO.RECEIVER) chltype(SDR) conname('127.0.0.1(15552)') XMITQ(RECEIVER)
      define channel(RECEIVER.TO.SENDER) chltype(RCVR) 
     " | runmqsc SENDER

endmqm RECEIVER
dltmqm RECEIVER
crtmqm RECEIVER
strmqm RECEIVER
echo "define ql(SENDER) usage(XMITQ)
      define listener(MYLSR) TRPTYPE(TCP) PORT(15552) CONTROL(QMGR)
      start listener(MYLSR)
      define channel(RECEIVER.TO.SENDER) chltype(SDR) conname('127.0.0.1(15552)') XMITQ(RECEIVER)
      define channel(SENDER.TO.RECEIVER) chltype(RCVR) 
     " | runmqsc RECEIVER

