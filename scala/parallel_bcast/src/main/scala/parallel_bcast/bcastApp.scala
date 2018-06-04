// Name : Sameer Deshmukh
// Student ID: 17M38101
// Parallel broadcast and convergecast code in scala.

import neko.PID
import neko.ProcessConfig
import neko.Event
import neko.util
import neko.util.TraversalClient
import neko.ReactiveProtocol

class BcastApp(p: ProcessConfig, root: PID = PID(0))
  extends ReactiveProtocol(p, "AppTraversal") with util.TraversalClient
  {
    override def preStart(): Unit =
    {
      if (me == root) initiate()
    }

    def onReceive = PartialFunction.empty[Event,Unit]

    def onSend = PartialFunction.empty[Event,Unit]
  }
