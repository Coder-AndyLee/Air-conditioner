from twisted.internet.protocol import Protocol, Factory
from twisted.internet import reactor

class Sever(Protocol):

    def __init__(self):
        self.priority = -1

    def connectionMade(self):
        self.factory.clients.append(self)
        print(self.factory.clients)

    def connectionLost(self, reason):
        self.factory.clients.remove(self)
        if self.priority == 0:
            print("Host has lost connection")
            for slave in self.factory.clients:
                message = "[{\"error\":\"Host was disconnected!\"}]"
                slave.message(message)

        elif self.priority == -1:
            print("One slave has lost connection")

    def dataReceived(self, data):
        a = data.split('-')
        if len(a) > 1:
            command = a[0]
            content = a[1]

            msg = ""
            if command == "choosePrio":
                if content[0] == "0":
                    for c in self.factory.clients:
                        if c.priority == 0:
                            msg = "[{\"error\":\"Host is existent.Can't exist two hosts!\"}]"
                            self.message(msg)

                    if len(msg) == 0:
                        self.priority = int(content[0])

                elif content[0] == "1":
                    self.priority = int(content[0])

            elif command == "msg":
                print(content)
                if self.priority == -1:
                    content = "[{\"error\":\"Please set priority first!\"}]"
                    self.message(content)
                    return

                if self.priority == 0:
                    for c in self.factory.clients:
                        if c != self and c.priority == 1:
                            c.message(content)

                else:
                    for c in self.factory.clients:
                        if c != self and c.priority == 0:
                            c.message(content)

            else:
                msg = "[{\"error\":\"Wrong command! Please try again\"}]"
                self.message(msg)


    def message(self, message):
        self.transport.write(message + '\n')


factory = Factory()
factory.protocol = Sever
factory.clients = []

reactor.listenTCP(8888, factory)
print "Sever Started"
reactor.run()