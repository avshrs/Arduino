m = 0b11110000 #a
f = 0b11001100 #b
v = 0b10101010 #c

#McpMemory[s] = McpMemory[s]^((~McpForce[s])&(value^McpMemory[s]));
print bin(m^((~f)&(v^m)))

print bin(((~f & v)|(m&f)))

print bin(((f | v)& (m|~f)))


https://www.electronics-tutorials.ws/boolean/bool_6.html
http://www.32x8.com/
