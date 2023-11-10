import numpy as np

def experiment(p, q, num_trials=10000):
    N_values = np.random.geometric(p, size=num_trials)
    Y_values = [np.random.binomial(N, q) for N in N_values]
    return np.mean(Y_values), np.var(Y_values)

p_values = q_values = np.arange(0.1, 1, 0.1)
mean_table = np.zeros((9, 9))
variance_table = np.zeros((9, 9))

for i, p in enumerate(p_values):
    for j, q in enumerate(q_values):
        mean, variance = experiment(p, q)
        mean_table[i, j] = mean
        variance_table[i, j] = variance

print("mean")
print("    q:  0.1     0.2     0.3     0.4     0.5     0.6     0.7     0.8     0.9")
print(" p   ------------------------------------------------------------------------")
for i in range(len(p_values)):
    print(f"{round(p_values[i],1):.1f}", "|  ", end = "")
    k = 0
    for j in range(len(q_values)):
        print(f"{round(mean_table[i][j],3):.3f}", end= "   ")
    print()
print()
print(mean_table)
print("\nvariance")
print("    q:  0.1     0.2     0.3     0.4     0.5     0.6     0.7     0.8     0.9")
print(" p   ------------------------------------------------------------------------")
for i in range(len(p_values)):
    print(f"{round(p_values[i],1):.1f}", "|  ", end = "")
    k = 0
    for j in range(len(q_values)):
        print(f"{round(variance_table[i][j],3):.3f}", end= "   ")
    print()

print()
print(variance_table)