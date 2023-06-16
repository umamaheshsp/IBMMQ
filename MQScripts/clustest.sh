#!/usr/bin/ksh

crtmqm MANAF1
strmqm MANAF1
echo "def listener(listenner) trptype(TCP) port(1415) control(qmgr) " | runmqsc MANAF1
echo "start listener(listenner) " | runmqsc MANAF1
echo "alter qmgr repos(clus_a) " | runmqsc MANAF1
echo "def chl(to.MANAF1) chltype(clusrcvr) conname('127.0.0.1(1415)') cluster(clus_a) replace " | runmqsc MANAF1
echo "def chl(to.MANAF2) chltype(clussdr)  conname('127.0.0.1(1416)') cluster(clus_a) replace " | runmqsc MANAF1
echo "def ql(clq) cluster(clus_a) defbind(notfixed) defpsist(yes)" | runmqsc MANAF1
#echo "alter qmgr CLWLUSEQ(LOCAL)" | runmqsc MANAF1

crtmqm MANAF2
strmqm MANAF2
echo "def listener(listenner) trptype(TCP) port(1416) control(qmgr) " | runmqsc MANAF2
echo "start listener(listenner) " | runmqsc MANAF2
echo "alter qmgr repos(clus_a) " | runmqsc MANAF2
echo "def chl(to.MANAF2) chltype(clusrcvr) conname('127.0.0.1(1416)') cluster(clus_a) replace " | runmqsc MANAF2
echo "def chl(to.MANAF1) chltype(clussdr)  conname('127.0.0.1(1415)') cluster(clus_a) replace " | runmqsc MANAF2
echo "def ql(clq) cluster(clus_a) defbind(notfixed) defpsist(yes)" | runmqsc MANAF2
#echo "alter qmgr CLWLUSEQ(LOCAL)" | runmqsc MANAF2

crtmqm MANAP1
strmqm MANAP1
echo "def listener(listenner) trptype(TCP) port(1417) control(qmgr) " | runmqsc MANAP1
echo "start listener(listenner) " | runmqsc MANAP1
echo "def chl(to.MANAP1) chltype(clusrcvr) conname('127.0.0.1(1417)') cluster(clus_a) replace " | runmqsc MANAP1
echo "def chl(to.MANAF1) chltype(clussdr)  conname('127.0.0.1(1415)') cluster(clus_a) replace " | runmqsc MANAP1
echo "def ql(clq) cluster(clus_a) defbind(notfixed) defpsist(yes)" | runmqsc MANAP1
#echo "alter qmgr CLWLUSEQ(LOCAL)" | runmqsc MANAP1

#crtmqm MANAP2
#strmqm MANAP2
#echo "def listener(listenner) trptype(TCP) port(1418) control(qmgr) " | runmqsc MANAP2
#echo "start listener(listenner) " | runmqsc MANAP2
#echo "def chl(to.MANAP2) chltype(clusrcvr) conname('127.0.0.1(1418)') cluster(clus_a) replace " | runmqsc MANAP2
#echo "def chl(to.MANAF1) chltype(clussdr)  conname('127.0.0.1(1415)') cluster(clus_a) replace " | runmqsc MANAP2
#echo "def ql(clq) cluster(clus_a) defbind(notfixed) defpsist(yes)" | runmqsc MANAP2
#echo "alter qmgr CLWLUSEQ(LOCAL)" | runmqsc MANAP2


