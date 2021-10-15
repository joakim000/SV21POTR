/* First tutorial example */
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
// #include <pthread.h>
  
// // A normal C function that is executed as a thread 
// // when its name is specified in pthread_create()
// void *myThreadFun(void *vargp)
// {
//     sleep(1);
//     printf("Printing GeeksQuiz from Thread \n");
//     return NULL;
// }
   
// int main()
// {
//     pthread_t thread_id;
//     printf("Before Thread\n");
//     pthread_create(&thread_id, NULL, myThreadFun, NULL);
//     pthread_join(thread_id, NULL);
//     printf("After Thread\n");
//     exit(0);
// }


/* Second tutorial example */
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <pthread.h>
  
// // Let us create a global variable to change it in threads
// int g = 0;
  
// // The function to be executed by all threads
// void *myThreadFun(void *vargp)
// {
//     // Store the value argument passed to this thread
//     int *myid = (int *)vargp;
  
//     // Let us create a static variable to observe its changes
//     static int s = 0;
  
//     // Change static and global variables
//     ++s; ++g;
  
//     // Print the argument, static and global variables
//     printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g);
// }
  
// int main()
// {
//     int i;
//     pthread_t tid;
  
//     // Let us create three threads
//     for (i = 0; i < 3; i++)
//         pthread_create(&tid, NULL, myThreadFun, (void *)&tid);
  
//     pthread_exit(NULL);
//     return 0;
// }

/* SO question:
    https://stackoverflow.com/questions/1352749/multiple-arguments-to-function-called-by-pthread-create
*/

#include <stdio.h>
#include <pthread.h>

struct arg_struct {
    int arg1;
    int arg2;
};

void *print_the_arguments(void *arguments)
{
    // struct arg_struct *args = (struct arg_struct *)arguments;
    struct arg_struct *args = arguments;

    printf("%d\n", args -> arg1);
    printf("%d\n", args -> arg2);
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    pthread_t some_thread;
    struct arg_struct args;
    args.arg1 = 5;
    args.arg2 = 7;

    if (pthread_create(&some_thread, NULL, &print_the_arguments, (void *)&args) != 0) {
        printf("Uh-oh!\n");
        return -1;
    }

    return pthread_join(some_thread, NULL); /* Wait until thread is finished */
}