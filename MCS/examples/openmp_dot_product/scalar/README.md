## Example

```
./run 100000000 2
pre-phase
pre-phase: 2.589516 s

running phase serial
sum = -5022.301831
running phase serial: 0.365274 s

running phase mutex
sum = -5022.301831
running phase mutex: 12.881751 s

running phase critical
sum = -5022.301831
running phase critical: 10.542174 s

running phase atomic
sum = -5022.301831
running phase atomic: 7.134792 s

running phase reduction
sum = -5022.301831
running phase reduction: 0.222731 s

post-phase
post-phase: 0.080509 s
```

*На меньших цифрах не так просто получить ускорение для такой задачи.*

Использование runner.h опционально, улучшения приветсвуются.

## valgrind

В зависимости от компилятора/ОС, valgrind может выдать что-то вроде
```
==8442== HEAP SUMMARY:
==8442==     in use at exit: 2,400 bytes in 6 blocks
==8442==   total heap usage: 13 allocs, 7 frees, 201,376 bytes allocated
==8442== 
==8442== LEAK SUMMARY:
==8442==    definitely lost: 0 bytes in 0 blocks
==8442==    indirectly lost: 0 bytes in 0 blocks
==8442==      possibly lost: 288 bytes in 1 blocks
==8442==    still reachable: 2,112 bytes in 5 blocks
==8442==         suppressed: 0 bytes in 0 blocks
==8442== Rerun with --leak-check=full to see details of leaked memory
==8442== 
==8442== For counts of detected and suppressed errors, rerun with: -v
==8442== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

Это нормально, особое внимание уделяетяся на **definitely lost** и **ERROR SUMMARY**