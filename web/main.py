#main.py
import web
import json
import time
import socket 
import sys


def make_text(string):
	return string

urls = ('/', 'index','/command', 'command')

#render = web.template.render('/usr/local/apache2/htdocs/templates/')   # TODO qui va messo il Path assoluto con APACHE 

render = web.template.render('templates/')# TODO qui va messo il Path assoluto con APACHE 


# PER APACHE
#application = web.application(urls, globals()).wsgifunc()
application = web.application(urls, globals())

my_form = web.form.Form(
                web.form.Textbox('', class_='textfield', id='textfield')
                )
                
		
monitors ={
			"error" : "No answer from the FTK monitoring system",
			"reason" : "No ftk_monitor_system running on host",
			"suggestion" : "Source the ftk package on the machine and start ftk_monitoring_system"
		  }
		  
threadError ={
			"error" : "Socket Creation Error",
			"reason" : "Impossible to establish connection with remother thread"
		  }

serverConnection = None

      
def connectToMonitoring():  

	global context

	print 'connecting to base '
	server = socket.socket()
	server.connect(('localhost',9999))
	print 'connected '
	portData = server.recv(1024)
	server.close()

	firstserverThread = socket.socket()
	firstserverThread.connect(('localhost',int(portData)))
	#context = firstserverThread.recv(1024)    
	return firstserverThread	

def commandExecution(command):  

	global serverConnection   
    
	print 'sending : ' + command
	sentchars = serverConnection.send(command)
	print 'sent : ' + str(sentchars) + 'bytes'
	print 'reading .. '
	rec = serverConnection.recv(4096)
	response = rec
	print 'readed ' + str(len(rec)) + 'bytes'
	while len(rec) == 4096:
		rec = serverConnection.recv(4096)
		if rec == '':
			raise RuntimeError("socket connection broken")
		
		response += rec
		print 'readed other ' + str(len(rec)) + 'bytes'
		
	print 'response is long' + str(len(response))
	return response


class command:
    
    def GET(self):
        user_data = web.data()
        for k, v in user_data:
			print 'eccoli'+ k + '=' + v
        print 'received some' 
        return risp

    def POST(self):
        global serverConnection
        user_data = web.input()
        command = user_data['command']
        print 'received '+ command
        response=json.dumps(monitors)
        # handshake, if is the first connection
        if serverConnection is None:
            try:
                serverConnection=connectToMonitoring()
            except:
                print 'Impossible to connect, returning empty' 
                serverConnection = None
                return json.dumps(threadError)
        # trying to submit a command
        try:
            response=commandExecution(command)
            if len(response) == 0 :
            	response=commandExecution(command)
        except:
            serverConnection = None
            pass
        
        return response
        
class index:
    def GET(self):
        hname = socket.gethostname()
        return render.AM_Template(hname, "   Waiting for Commands")
        
    def POST(self):
        form = my_form()
        form.validates()
        s = form.value['textfield']
        return make_text(s+s)

if __name__ == '__main__':
    #application.run()
    application.run()
