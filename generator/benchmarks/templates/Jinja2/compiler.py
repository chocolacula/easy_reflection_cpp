from jinja2 import Environment, FileSystemLoader
import time

start_time = time.time()

file_loader = FileSystemLoader('templates')
env = Environment(loader=file_loader)

env.compile_templates('templates_compiled', zip=None)

print("Execution takes {:.4f} seconds".format(time.time() - start_time))
