package parallel_bcast.parallel_bcast

import neko._

object ParallelBcast
{
  // define case classes
  case class Go(from: PID, to: PID, payload: Long) extends UnicastMessage
  case class Back(from: PID, to: PID, payload: Long) extends UnicastMessage
  case class Start(from: PID, to: PID) extends UnicastMessage
}

class ParallelTraversal(c: ProcessConfig) extends ReactiveProtocol(c, "bcast ccast")
{
  import ParallelBcast._

  private val parent = None
  private val children = None
  //private val expected_msgs // how to get number of neighbours of process?

  def onSend = {
    case _ => SEND(Go(me, PID(0), 1))
  }

  listenTo(classOf[Go])
  listenTo(classOf[Back])
  listenTo(classOf[Start])
  def onReceive = {
    case _ => SEND(Go(me, PID(0), 1))
    // case Start(from, to) =>
    //   //
    // case _ =>
    //   //
  }
}

object ParallelBcastMain extends Main(topology.Grid(4,4))(
  ProcessInitializer { p =>
    val app = new BcastApp(p)
    val traverse = new ParallelTraversal(p)
    app --> traverse
  }
)

