1. Nicole Wang, Lin Li
2. Program runs correct when next pop / push will not exteed the low / high water mark. 
   If the order changes and cause the next pop/push action will exteed the low/high water mark, it will waiting until the next action will not cause water mark be exceed. 
   However, since it's single thread, no other action will be preformed so the program will infinitely waiting there, cause deadlock.
3. The program runs correct with no deadlock because according to the instruction, pop only happens after enqueue/push, and since we use detach the "pop" part into another thread,
   the main function won't wait until it finishes. Instead main will return after 5s (since we let it sleep for 5s). Therefore, the infinite waiting in the thread won't cause problem in the program running.
4. We add a local boolean variable in run_service method, to check if everything is popped from list, in order to avoid last null (0) element to be printed out.
   Enqueue-active-deactive order: correct
   Enqueue can not be performed after deactive since isShutDown flag is true and there's no where we can flip it to false. Enqueue(push) or pop will not be performed if isShutDown is true.
   Active can be performed after deactive but nothing will be poped because isShutDown flag will be flipped to true and stay true after deactive so no future push/pop will be performed.
   In general, no push/pop action can be performed after deactive because isShutDown flag will be set to true forever during deactive.
5.1
enqueue obj2
push: 1
push: 2
finish enqueue obj2. obj2 active
push: 1
chain enqueue: 1
push: 2
chain enqueue: 2
finish active obj2. obj2 deactive
finish deactive obj2. obj1 active
pop: 1
pop: 2
finish active obj1. obj1 deactive
finish deactive obj1

5.2 Pop/push method can be preformed in chain from one active_object to another active_object. 
    When there's object chain to current object, the run_service method not only pop elements from current list, but also push them to the chain object's list in one action.

   
   