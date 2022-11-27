from absurd_gen.generator import ABSURD_gen

# Put there the path to the data/version directory. Add a trailing /
DATA_PATH="/path/to/data/0.2/"

# Configure the generator
acas_gen = ABSURD_gen("acas", platform="odroid-h2", data_path=DATA_PATH)

# Set the seed (if necessary, otherwise skip it)
acas_gen.set_seed(12345)

# Print the (constant) WCET:
print("The WCET is:", acas_gen.get_wcet())

# Generate some random numbers:
print("1st random exec. time:", acas_gen.rand())
print("2nd random exec. time:", acas_gen.rand())
print("3rd random exec. time:", acas_gen.rand())
print("10 random exec. times:", acas_gen.rand_vector(10))
