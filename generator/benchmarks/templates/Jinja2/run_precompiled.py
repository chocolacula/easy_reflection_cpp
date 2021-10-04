from jinja2 import Environment, ModuleLoader
import time

start_time = time.time()

module_loader = ModuleLoader('templates_compiled')
env = Environment(loader=module_loader)

template = env.get_template('enum.jinja2')

name = 'TheEnumClass'
enums = ['White', 'Red', 'Green', 'Blue', 'Black']

rendered = template.render(name=name, enums=enums)

text_file = open("generated/enum.er.h", "w")
text_file.write(rendered)
text_file.close()

print("Execution takes {:.4f} seconds".format(time.time() - start_time))
