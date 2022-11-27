import random
import csv

DEFAULT_DATA_PATH = "../data/0.2"

class ABSURD_gen:
    """
    This class represents a generator based on the ABSURD
    benchmark data measured from real platforms.
    
    You can create multiple ABSURD_gen classes, RNGs are split
    and can work simultaneously.
    """
    def __init__(self, benchmark_name, benchmark_class="b", platform="nucleo-f746zg", data_path=DEFAULT_DATA_PATH):
        """
        Parameters
        ----------
        benchmark_name : str
            The name of the benchmark (refer to the table under docs/ directory)
        benchmark_class : str, optional
            Valid values: a, b, c, d, e
            NOTE: Not all platforms implement all the classes.
            Default value is b.
        platform : str, optional
            Valid values: nucleo-f746zg, odroid-h2
            Default value is nucleo-f746zg
        data_path : str, optional
            Path to the data/ directory.
            Default value is "../data/"

        """ 
        self.benchmark_name = benchmark_name
        self.benchmark_class= benchmark_class
        self.platform       = platform
        self.py_random      = random.Random()
        self.data_path      = data_path
        self.bench_exec_times = []
        self._open_bench_data()

    def _open_bench_data(self):
        filename = self.data_path + "exec_times/" + self.platform + "/class_" + self.benchmark_class.lower() + "/" + self.benchmark_name.lower() + ".dat"
        with open(filename) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            for row in csv_reader:
                self.bench_exec_times.append(float(row[0]))
            
            self.bench_exec_times.sort()
            self.wcet = self.bench_exec_times[-1]
        
    def set_seed(self, seed):
        """
        Set a seed to the internal RNG
        """
        self.py_random.seed(seed)
        
    def get_wcet(self):
        """
        Return the maximum generable execution time.
        It is based on the Worst-Case Observed Time of the benchmark analysis.
        This is NOT generate from the pWCET analysis.
        """
        return self.wcet
        
    def rand(self):
        """
        Get a single execution time (as float)
        """
        r = self.py_random.randint(0, len(self.bench_exec_times)-1)
        return self.bench_exec_times[r]
        
    def rand_vector(self,size):
        """
        Get a sequence of size execution times (as list of float)
        """
        return [self.rand() for x in range(size)]
