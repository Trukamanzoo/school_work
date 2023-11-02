import random

# Initialize the parameters
n = 7
p = 0.6
q = 0.7
num_trials = 100000

# Initialize the counters for the joint PMF and the marginal PMFs
joint_pmf_counter = [[0 for _ in range(n+1)] for _ in range(n+1)]
marginal_pmf_X = [0 for _ in range(n+1)]
marginal_pmf_Y = [0 for _ in range(n+1)]

# Run the simulation
for _ in range(num_trials):
    X = 0  # Number of weeks Joe plays
    Y = 0  # Number of weeks Joe wins

    # Simulate n weeks of play
    for _ in range(n):
        if random.random() < p:  # Joe plays this week
            X += 1
            if random.random() < q:  # Joe wins this week
                Y += 1

    # Update the counters
    joint_pmf_counter[X][Y] += 1
    marginal_pmf_X[X] += 1
    marginal_pmf_Y[Y] += 1

# Compute the estimated PMFs
joint_pmf_estimated = [[count/num_trials for count in row] for row in joint_pmf_counter]
marginal_pmf_X_estimated = [count/num_trials for count in marginal_pmf_X]
marginal_pmf_Y_estimated = [count/num_trials for count in marginal_pmf_Y]

# Compute the conditional PMFs
conditional_pmf_X_given_Y_estimated = [[joint_pmf_estimated[x][y]/marginal_pmf_Y_estimated[y] if marginal_pmf_Y_estimated[y] > 0 else 0 for y in range(n+1)] for x in range(n+1)]
conditional_pmf_Y_given_X_estimated = [[joint_pmf_estimated[x][y]/marginal_pmf_X_estimated[x] if marginal_pmf_X_estimated[x] > 0 else 0 for y in range(n+1)] for x in range(n+1)]

# Print the results (rounded to four decimal places)
print("Joint PMF of X and Y:")
print("   y:   0       1       2       3       4       5       6       7")
print("x ------------------------------------------------------------------")
row_num = 0
for row in joint_pmf_estimated:
    print(row_num, " |  ", end = "")
    i = 0
    for prob in row:
        if i == row_num + 1:
            break;
        print(f"{round(prob,4):.4f}", end = "  ")
        i += 1
    print()
    row_num += 1
print()
print("Conditional PMF of X given Y:")
print("   y:   0       1       2       3       4       5       6       7")
print("x ------------------------------------------------------------------")
row_num = 0
for row in conditional_pmf_X_given_Y_estimated:
    print(row_num, " |  ", end = "")
    i = 0
    for prob in row:
        if i == row_num + 1:
            break;
        print(f"{round(prob,4):.4f}", end = "  ")
        i += 1
    print()
    row_num += 1
print()
print("Conditional PMF of Y given X:")
print("   y:   0       1       2       3       4       5       6       7")
print("x ------------------------------------------------------------------")
row_num = 0
for row in conditional_pmf_Y_given_X_estimated:
    print(row_num, " |  ", end = "")
    i = 0
    for prob in row:
        if i == row_num + 1:
            break;
        print(f"{round(prob,4):.4f}", end = "  ")
        i += 1
    print()
    row_num += 1
