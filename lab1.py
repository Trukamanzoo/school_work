import random

def trial_one():
    p = 0.5
    n = 100
    bob_trials = 0
    #number of trials
    for i in range(1000): 
        bob_heads = 0
        alice_heads = 0
        # bob flips n+1 coins
        for j in range(n+1):
            coin = random.random()
            if coin < p: # from LoadedCoin.py
                bob_heads += 1
        
        # alice flips n coins
        for j in range(n):
            coin = random.random()
            if coin < p:
                alice_heads += 1
                
        # check
        if bob_heads > alice_heads:
            bob_trials += 1
    
    rf = bob_trials/1000
    return(rf)

def trial_two():
    print("--------------------------")
    print(" p      relative frequency")
    print("--------------------------")
    trials = 1000
    p = [0.2,0.3,0.4,0.5,0.6,0.7,0.8]                         # probability       
    n = 100                                     # n coins
    num_coins = 2*n + 1                         # total number of coins
    
    for prob in p:                              # for each probability
        bob_trials = 0                          # trials that bob has more heads than alice
        alice_trials = 0                        # trials that alice has more heads than bob
        for i in range(trials):                 # do 1000 trials
            bob_heads = 0
            alice_heads = 0
            
            # bob coin tosses
            for j in range(n+1):
                coin = random.random()
                if coin < prob:
                    bob_heads += 1
                    
            # alice coin tosses
            for j in range(n):
                coin = random.random()
                if coin < prob:
                    alice_heads += 1
                    
            if bob_heads > alice_heads: # if Bob has more heads than Alice, then add it to his trials
                bob_trials += 1
        
        rf = bob_trials/trials          # relative frequency       
        print(prob, "    ", rf)
    return()

for i in range(1, 8):
    print("Trial Number:", i,  "Relative Frequency:", trial_one())
trial_two()
