# Rutronik Elektronische Bauelemente GmbH Disclaimer: The evaluation board
# including the software is for testing purposes only and,
# because it has limited functions and limited resilience, is not suitable
# for permanent use under real conditions. If the evaluation board is
# nevertheless used under real conditions, this is done at one’s responsibility;
# any liability of Rutronik is insofar excluded

# Apply offset to the array
def apply_offset(y, offset):
    retval = []
    for i in range(len(y)):
        retval.append(y[i] + offset)
    return retval

def multiply(y, factor):
    retval = []
    for i in range(len(y)):
        retval.append(y[i] * factor)
    return retval

def rotate(array):
    line_count = len(array)
    col_count = len(array[0])

    retval = []

    for col_index in range(col_count):
        rotated_col = []
        for line_index in range(line_count):
            rotated_col.append(array[line_index][col_index])
        retval.append(rotated_col)

    return retval

def get_max(y):
    max = y[0]
    for i in range(len(y)):
        if (y[i] > max):
            max = y[i]
    return max

def get_max_and_index(y):
    max = y[0]
    index = 0
    for i in range(len(y)):
        if (y[i] > max):
            index = i
            max = y[i]
    return max, index

def get_matrix_max(matrix):
    maxValue = matrix[0][0]
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            if (matrix[i][j] > maxValue):
                maxValue = matrix[i][j]
    return maxValue