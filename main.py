
from mymessage_pb2 import MsgRequest


msg = MsgRequest()
msg._id = 10
msg.name = "ae1554"
msg.query = "UPDATE docs SET done=true WHERE status='1';"

filename = "message1.pb"
with open(filename, "w") as fd:
  fd.write(msg.SerializeToString())
  print("Wrote %s" % filename)