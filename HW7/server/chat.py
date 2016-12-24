#/usr/bin/env python
#coding=utf8

import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web

dict_for_chat = {}

from tornado.options import define, options
define("port", default=8001, help="run on the given port", type=int)

class ReverseHandler(tornado.web.RequestHandler):
    def get(self, input):
        global dict_for_chat
        output = ""
        data_mapped = str(input).strip().split("d4d")
        if len(data_mapped) != 5:
            output = str(data_mapped)
        else:
            chat_id, name_spc, cond, time, message = data_mapped
            if cond == "1":
                if dict_for_chat.has_key(chat_id):
                    dict_for_chat[chat_id] += chat_id + " "+ time + " " + name_spc + "\n" + message + "\n"
                    output = dict_for_chat[chat_id]
                else:
                    dict_for_chat[chat_id] = chat_id + " " + time + " " + name_spc + "\n" + message + "\n"
                    output = dict_for_chat[chat_id]
            else:
                if dict_for_chat.has_key(chat_id):
                    output = dict_for_chat[chat_id]
                else:
                    output = "welcome to a new chat_room!"
            
        self.write(output)
        
if __name__ == "__main__":
    tornado.options.parse_command_line()
    app = tornado.web.Application(
        handlers=[
            (r"/(\w+)", ReverseHandler),
        ]
    )
    http_server = tornado.httpserver.HTTPServer(app)
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()
