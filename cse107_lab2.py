import random
a_balls = [10, 20, 30, 40, 50]
print("----------------------------------------------")
print(" #azure  #carmine   proportion ending in azure")
print("----------------------------------------------")
for j in a_balls:
    azure_total = 0
    for i in range(2000):
        a = j
        c = 100-a
        total = a + c
        predecessor = -1; 
        while(total > 0):
            coin = random.random()
            p = a/total #probablity of azure ball
            if coin < p: # if azure ball
                if predecessor == -1: # if there is no predecessor
                    predecessor = 0 # set predecessor as Azure
                elif predecessor == 1: #if predecessor is Carmine
                    predecessor = -1 # restart the process
                    continue;
                #print("Azure: probablity was ", p)
                a -= 1
                total -= 1
            else:   # if carmine ball
                if predecessor == -1: # if there is no predecessor
                    predecessor = 1 # set predecessor as Azure
                elif predecessor == 0: #if predecessor is Carmine
                    predecessor = -1 # restart the process
                    continue;
                #print("Carmine: probablity was ", 1-p)
                c -= 1
                total -= 1
        if predecessor == 0:
            azure_total += 1
    
    rf = "{:.4f}".format(azure_total/2000)
    print("  ", j, "    ", 100-j, "      ", rf)
