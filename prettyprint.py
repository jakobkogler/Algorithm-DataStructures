import re

def get_vector_element(vec, index):
    t = gdb.types.get_basic_type(vec.type)
    return gdb.parse_and_eval('(*(%s*)(%s))[%d]' % (t, vec.address, index))

def get_vector_size(vec):
    eval_string = "(*("+str(vec.type)+"*)("+str(vec.address)+")).size()"
    return int(gdb.parse_and_eval(eval_string))

class ModularPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        return str(self.val['value'])# + "m"

class PolynomialPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        coeffs = self.val['coeffs']
        l = []
        sz = get_vector_size(coeffs)
        for i in range(min(sz, 10)):
            elem = get_vector_element(coeffs, i)
            l.append(f"{elem}*x^{i}")
        if sz > 10:
            l.append("...")
        return "Polynomial(" + ' + '.join(l) + ")"



def modular_loader(val):
    t = str(gdb.types.get_basic_type(val.type))
    if re.match(r"Modular<\d+>", t):
        return ModularPrinter(val)
    if re.match(r"Polynomial<.*>", t):
        return PolynomialPrinter(val)

gdb.pretty_printers.append(modular_loader)
