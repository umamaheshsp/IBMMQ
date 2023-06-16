 grep CurrentQDepthV mqapis.out|awk '{print $2 " " $3 " " $6}'|sort -k3
