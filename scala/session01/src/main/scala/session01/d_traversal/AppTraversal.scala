package session01.d_traversal

import neko._
import neko.util.TraversalClient

/**
  * Created by defago on 2017/04/12.
  */
class AppTraversal(p: ProcessConfig, root: PID = PID(0))
  extends ReactiveProtocol(p, "AppTraversal") with util.TraversalClient
  {
    import TraversalClient._

    override def preStart(): Unit =
    {
      if (me == root) initiate()
    }

    def onReceive = {
      case Visit => println(s"${me.name} :> VISIT $me")
      case Done  => println(s"${me.name} :> DONE")
    }

    def onSend = PartialFunction.empty[Event,Unit]
  }
