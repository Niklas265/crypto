# Aufgabe 3b

### Welche Klassen der Crypto++-Bibliothek werden verwendet?

'Socket' included from <socketft.h>, which is part of CryptoPP

### Wie ist der Quellcode des Servers aufgebaut?

...

### Welche Befehle sind notwendig, um den Server zu starten? In welcher Reihenfolge müssen die Befehle ausgeführt werden?

// Create the server socket  
server.Create();  
// Bind to the given port  
server.Bind(port);  
// Listen on the port and wait for an incoming connection  
server.Listen();  
if (server.Accept(client) == true) {...

### Wie ist der Quellcode des Clients aufgebaut?

...

### Welche Befehle sind notwendig, um den Client zu starten? In welcher Reihenfolge müssen die Befehle ausgeführt werden?

Socket client;  
// Creating a socket ...  
client.Create();  
// ... and connecting to the server  
client.Connect(host.c_str(), port);  

# Aufgabe 4

### Wie heißen die Methoden, die den Teil der Kommunikation von Alice, Bob beziehungsweise Oscar beinhalten werden?

- DHProtocol::alice
- DHProtocol::bob
- DHProtocol::oscar


### An welcher Stelle werden die öffentlichen Parameter des Diffie-Hellman Protokolls eingelesen? Wie heißen die entsprechenden Variablen?

DHProtocol::DHProtocol(const string& param_file):

q = encoder.getInteger(0);  
p = encoder.getInteger(1);  
alpha = encoder.getInteger(2);  