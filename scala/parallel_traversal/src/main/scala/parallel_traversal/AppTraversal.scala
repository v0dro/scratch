package parallel_traversal.parallel_traversal

import neko.ProcessConfig
import neko.ReactiveProtocol
import neko.Event
import neko.PID
import neko.util
import neko.util.TraversalClient

class AppTraversal(p: ProcessConfig, root: PID = PID(0))
  extends ReactiveProtocol(p, "AppTraversal") with TraversalClient
  {
    override def preStart(): Unit =
    {
      initiate()
    }

    def onReceive = PartialFunction.empty[Event,Unit]

    def onSend = PartialFunction.empty[Event,Unit]
  }
