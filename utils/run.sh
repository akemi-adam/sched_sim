for i in $(seq 1000)
do
    ./main_fifo 100 | grep TME
done
