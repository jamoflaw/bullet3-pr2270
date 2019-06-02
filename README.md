# Building

> Note that this is from discussion on PR 2270 on bulletphysics/bullet3. [https://github.com/bulletphysics/bullet3/pull/2270](https://github.com/bulletphysics/bullet3/pull/2270).

Set DEPENDS_ROOT to the install path of the bullet libs - this allows CMake to find it.

In source builds are disabled so create a dir build inside the source directory, and run ```cmake ..```. This will generate a makefile. To build run ```make```.

```
RUN 1:
==873== Process terminating with default action of signal 6 (SIGABRT)
==873==    at 0x59D8E97: raise (raise.c:51)
==873==    by 0x59DA800: abort (abort.c:79)
==873==    by 0x59CA399: __assert_fail_base (assert.c:92)
==873==    by 0x59CA411: __assert_fail (assert.c:101)
==873==    by 0x21DAAB: btAlignedObjectArray<btThreadSupportPosix::btThreadStatus>::operator[](int) (btAlignedObjectArray.h:177)
==873==    by 0x21D344: btThreadSupportPosix::waitForResponse() (btThreadSupportPosix.cpp:250)
==873==    by 0x21D3D6: btThreadSupportPosix::waitForAllTasks() (btThreadSupportPosix.cpp:266)
==873==    by 0x21AB8C: btTaskSchedulerDefault::waitForWorkersToSleep() (btTaskScheduler.cpp:629)
==873==    by 0x219F63: btTaskSchedulerDefault::~btTaskSchedulerDefault() (btTaskScheduler.cpp:448)
==873==    by 0x21A07F: btTaskSchedulerDefault::~btTaskSchedulerDefault() (btTaskScheduler.cpp:465)
==873==    by 0x16EC07: main (in /home/james/Documents/BulletMTTest/build/bullet_mt_test)

RUN 2:
==984== Thread 2:
==984== Invalid write of size 8
==984==    at 0x4E4C787: __new_sem_wait_slow.constprop.0 (sem_waitcommon.c:212)
==984==    by 0x21CECB: threadFunction(void*) (btThreadSupportPosix.cpp:181)
==984==    by 0x4E436DA: start_thread (pthread_create.c:463)
==984==    by 0x5ABB88E: clone (clone.S:95)
==984==  Address 0x62513a0 is 0 bytes inside a block of size 32 free'd
==984==    at 0x4C3123B: operator delete(void*) (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==984==    by 0x21CE9E: destroySem(sem_t*) (btThreadSupportPosix.cpp:171)
==984==    by 0x21D6E9: btThreadSupportPosix::stopThreads() (btThreadSupportPosix.cpp:307)
==984==    by 0x21CD4F: btThreadSupportPosix::~btThreadSupportPosix() (btThreadSupportPosix.cpp:130)
==984==    by 0x21CDA9: btThreadSupportPosix::~btThreadSupportPosix() (btThreadSupportPosix.cpp:133)
==984==    by 0x219FE0: btTaskSchedulerDefault::~btTaskSchedulerDefault() (btTaskScheduler.cpp:457)
==984==    by 0x21A07F: btTaskSchedulerDefault::~btTaskSchedulerDefault() (btTaskScheduler.cpp:465)
==984==    by 0x16EC07: main (in /home/james/Documents/BulletMTTest/build/bullet_mt_test)
==984==  Block was alloc'd at
==984==    at 0x4C3017F: operator new(unsigned long) (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==984==    by 0x21CDCE: createSem(char const*) (btThreadSupportPosix.cpp:159)
==984==    by 0x21D4CE: btThreadSupportPosix::startThreads(btThreadSupportInterface::ConstructionInfo const&) (btThreadSupportPosix.cpp:282)
==984==    by 0x21CCF0: btThreadSupportPosix::btThreadSupportPosix(btThreadSupportInterface::ConstructionInfo const&) (btThreadSupportPosix.cpp:124)
==984==    by 0x21D800: btThreadSupportInterface::create(btThreadSupportInterface::ConstructionInfo const&) (btThreadSupportPosix.cpp:350)
==984==    by 0x21A100: btTaskSchedulerDefault::init() (btTaskScheduler.cpp:470)
==984==    by 0x21925A: btCreateDefaultTaskScheduler() (btTaskScheduler.cpp:781)
==984==    by 0x16E2BE: main (in /home/james/Documents/BulletMTTest/build/bullet_mt_test)
==984== 
==984== Conditional jump or move depends on uninitialised value(s)
==984==    at 0x4E4C78F: __new_sem_wait_slow.constprop.0 (sem_waitcommon.c:212)
==984==    by 0x21CECB: threadFunction(void*) (btThreadSupportPosix.cpp:181)
==984==    by 0x4E436DA: start_thread (pthread_create.c:463)
==984==    by 0x5ABB88E: clone (clone.S:95)
==984== 
==984== 
==984== HEAP SUMMARY:
==984==     in use at exit: 0 bytes in 0 blocks
==984==   total heap usage: 1,089 allocs, 1,089 frees, 4,664,536 bytes allocated
==984== 
==984== All heap blocks were freed -- no leaks are possible
==984== 
==984== For counts of detected and suppressed errors, rerun with: -v
==984== Use --track-origins=yes to see where uninitialised values come from
==984== ERROR SUMMARY: 6 errors from 2 contexts (suppressed: 0 from 0)
```