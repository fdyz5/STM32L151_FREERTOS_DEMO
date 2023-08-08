# 版本更新，修改，移植说明文档


#### 说明: 
	1.遵循markdown书写语法，详细的可以参考菜鸟教程学习：https://www.runoob.com/markdown/md-tutorial.html
	2.cJason选择使用Keil内置的Jansson，具体的可以参考API文档：http://jansson.readthedocs.io/en/latest/apiref.html
	3.FreeRTOSv10.3.1版本选择最新的版本：https://www.freertos.org/a00104.html
	4.修改此工程需要添加日期、功能删减、修改人员、联系方式等说明

参考资料：下载地址：https://www.st.com/zh/microcontrollers-microprocessors/stm32l151re.html#
	STM32Lxx参考手册.pdf
	数据手册_L151.pdf
	
	
## 版本号 V1.0


	作    者 ：woody
	联系方式 ：QQ 2490006131
	修改时间 ：2020/09/07
	最新样例 ：
	功能说明 ：STM32L151大容量支持
			   FreeRTOSv10.3.1 最新版本支持
			   cJson支持（keil自带的jansson）
			   添加Cortex-M3 特有的位绑定操作GPIO存放在common.h
			   添加串口1的DEBUG模式，在MainConfig.h选择是否启用，默认串口波特率115200
			   所有Application层任务创建的优先级、堆栈大小、句柄，以及BSP层的NVIC设置抢占优先级要求都存放在MainConfig.h
			   驱动、任务、主函数封层编写，不允许合成一个.c文件，各层之间使用接口函数，添加完整接口说明
			   延时函数选择使用滴答时钟（未经严格测试）
			   低功耗模式选择低功耗睡眠模式，详情可以参考手册113页，理论上频率越低，功耗越低
		
/*******************************************************************************/
	在嵌入式系统中采用FreeRTOS进行任务设计时，创建任务需要注意以下几点：

1.任务优先级：FreeRTOS中任务的执行是基于任务优先级的，优先级越高的任务将获得更多的CPU时间。在创建任务时，需要为每个任务指定一个合适的优先级，确保任务按预期的顺序执行。

2.栈大小：每个任务在运行时需要使用一定的栈空间来保存局部变量、函数调用等。在创建任务时，需要指定每个任务的栈大小，确保栈空间足够大，避免栈溢出导致系统崩溃。

3.任务函数：每个任务需要有一个任务函数，即任务的入口点。任务函数在任务创建后会被调用，任务的实际功能逻辑应该在任务函数中实现。

4.任务句柄：创建任务后，FreeRTOS会返回一个任务句柄，可以通过该句柄来操作任务，如挂起、恢复、删除等操作。

5.堆栈分配方法：FreeRTOS支持静态分配和动态分配任务的堆栈。静态分配需要在编译时确定每个任务的栈大小，而动态分配可以在运行时动态分配栈空间。根据具体的系统需求和资源情况，选择合适的堆栈分配方法。

6.任务通信与同步：在多任务系统中，任务之间通常需要进行通信和同步。FreeRTOS提供了多种机制，如队列、信号量、互斥锁等，用于实现任务之间的数据交换和同步操作。

7.任务堵塞：任务在执行过程中可能会出现等待外部事件或资源的情况，此时可以使用任务堵塞的方式来暂停任务的执行，待条件满足后再继续执行。

8.错误处理：在创建任务时，需要检查任务创建是否成功，并处理可能的错误情况，例如任务句柄是否有效等。

总之，在使用FreeRTOS创建任务时，需要仔细考虑任务的优先级、栈大小、任务函数等参数，并确保任务之间的通信和同步能够正确地实现，以保证系统的稳定运行和正确功能。
/******************************************************************************/

/******************************************************************************/
确定需要创建任务的数量是嵌入式系统设计中非常重要的一步，它需要综合考虑系统的功能需求、硬件资源、任务优先级、响应时间等因素。以下是一些指导步骤来帮助确定需要创建任务的数量：

1.确定功能模块：首先，明确系统的功能模块，将系统划分为不同的功能块或模块。每个功能模块可以看作是一个潜在的任务。

2.任务划分：对于每个功能模块，进一步划分出需要并发执行的任务。这些任务可以是独立的功能模块，也可以是某些功能模块之间需要进行并发处理的子任务。

3.任务优先级：确定每个任务的优先级，优先级较高的任务将获得更多的CPU时间。优先级的确定要根据任务的重要性和响应时间要求来决定。

4.任务交互：考虑任务之间的交互和通信需求。如果任务之间需要共享数据或进行同步，需要相应地设置任务之间的通信机制，如队列、信号量等。

5.硬件资源：评估系统的硬件资源情况，包括CPU性能、内存、I/O接口等。确保系统可以满足所有任务的执行要求，并留有一定的余量。

6.响应时间：对于实时系统，需要考虑任务的响应时间要求。如果某些任务有较紧迫的响应时间需求，需要将它们设置为较高优先级的任务。

7.系统复杂性：考虑系统的复杂性和任务调度的开销。任务数量的增加会增加任务调度的开销，因此需要在功能需求和系统复杂性之间进行平衡。

8.测试和调试：创建适量的任务后，对系统进行测试和调试，确保任务能够正确地协同工作并满足系统需求。
/******************************************************************************/
在FreeRTOS中，任务的运行是由调度器（scheduler）来管理和控制的。调度器会根据任务的优先级和调度策略来决定当前应该运行哪个任务。为了保证每个任务都能得到执行，需要遵循以下几个原则：

1.任务优先级设置合理：为每个任务分配适当的优先级，确保优先级较高的任务能够及时得到执行。优先级越高的任务将优先被调度器选中执行。

2.任务的执行时间：每个任务应该在一个合理的时间范围内完成，避免某个任务占用太长时间导致其他任务得不到执行。

3.避免任务阻塞：任务在执行过程中尽量避免阻塞，比如不要使用延时函数或者长时间等待信号量。如果必须要等待某个事件发生，可以使用非阻塞的方式，如轮询方式。

4.使用合适的调度策略：FreeRTOS支持多种调度策略，如抢占式调度和协同式调度。在抢占式调度下，优先级较高的任务可以抢占优先级较低的任务，确保高优先级任务能够及时得到执行。在协同式调度下，任务需要主动让出CPU的使用权，因此需要设计良好的任务间协作方式，避免某个任务长时间霸占CPU。

5.使用合适的任务通信方式：多个任务之间可能需要进行通信和同步操作。使用合适的任务通信机制，如消息队列、信号量、事件标志组等，可以确保任务之间的协作和资源共享。

6.使用适当的堆栈大小：每个任务都需要一定的堆栈空间来保存任务的上下文和局部变量。确保为每个任务分配足够的堆栈空间，防止堆栈溢出问题。

7.避免优先级反转：优先级反转是一个常见的问题，可能导致优先级较高的任务因为等待优先级较低的任务而无法执行。使用优先级继承或者优先级屏蔽等技术可以解决优先级反转问题。
/******************************************************************************/

队列(queue)
二进制信号量(binary semaphores)
计数信号量(counting semaphores)
互斥量(mutexes)
递归互斥量、递归锁(recursive mutexes)
事件组(event groups)
任务通知(task notifications)