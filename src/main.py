
from mymessage_pb2 import MsgRequest, SuperMessage, ListMessage


msg = MsgRequest()
msg._id = 10
msg.name = "ae1554"
msg.query = "UPDATE docs SET done=true WHERE status='1';"

filename = "message1.pb"
with open(filename, "w") as fd:
  fd.write(msg.SerializeToString())
  print("Wrote %s" % filename)


supermsg = SuperMessage()
supermsg.row = 2
supermsg.sub.MergeFrom(msg)

filename = "message2.pb"
with open(filename, "w") as fd:
  fd.write(supermsg.SerializeToString())
  print("Wrote %s" % filename)


listmsg = ListMessage()
m1 = listmsg.messages.add()
m1._id = 1345
m1.name = "001345"
m1.query = "DELETE from docs;"
m2 = listmsg.messages.add()
m2._id = 1346
m2.name = "001346"
m2.query = "SELECT COUNT(*) FROM docs;"

filename = "message3.pb"
with open(filename, "w") as fd:
  fd.write(listmsg.SerializeToString())
  print("Wrote %s" % filename)
