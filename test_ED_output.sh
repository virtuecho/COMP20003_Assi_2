#!/usr/sbin/bash
for i in 1 15 100 1000 full
do
    echo RUNNING: valgrind ./dict1 1 tests/dataset_$i.csv ED_expected/test$i.out
    valgrind ./dict1 1 tests/dataset_$i.csv ED_expected/test$i.out < input/test$i.in > ED_expected/test$i.stdout.out
    diff expected/test$i.out ED_expected/test$i.out
    diff expected/test$i.stdout.out ED_expected/test$i.stdout.out
    read -p "Press enter to continue"
done

for i in 1 2 3
do
    echo RUNNING: valgrind ./dict1 2 tests/dataset_del.csv ED_expected/del$i.out
    valgrind ./dict1 2 tests/dataset_del.csv ED_expected/del$i.out < input/del$i.in > ED_expected/del$i.stdout.out
    diff expected/del$i.out ED_expected/del$i.out
    diff expected/del$i.stdout.out ED_expected/del$i.stdout.out
    read -p "Press enter to continue"
done

