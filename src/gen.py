
from oneof_pb2 import TopMsg, MSG_TYPE_1, MSG_TYPE_2

msg = TopMsg()
msg.sfield = "this is the top"
msg.ifield = 4
msg.type = MSG_TYPE_1

msg.sub1.text = "this is sub1"
msg.sub1.val  = 111

filename = "data/message1.pb"
with open(filename, "w") as fd:
  fd.write(msg.SerializeToString())
  print(msg)
  print("Wrote %s" % filename)
