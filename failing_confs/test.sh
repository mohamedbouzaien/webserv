for f in *.conf
do
    echo "Processing $f"
    cat -n $f
    ../webserv $f &
    pid=$!
    sleep 0.2
    kill ${pid}
    wait $! 2> /dev/null #silence kill output
    echo "---------------------------------"
done
