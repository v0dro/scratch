package parallel_bcast.parallel_bcast

import neko._

class ParallelTraversal(c: ProcessConfig) extends ReactiveProtocol(c, "bcast ccast")
{
  private val parent = None
  private val children = None
  private val expected_msgs = 

  def onSend = {

  }

  listenTo(classOf[Token])
  def onReceive = {

  }
}

object ParallelBcast
{
  // define case classes
  case class Go(from: PID, to: PID, payload: Long) extends UnicastMessage
  case class Back(from: PID, to: PID, payload: Long) extends UnicastMessage
  case class Start(from: PID, to: PID) extends UnicastMessage
}

object ParallelBcastMain extends Main(topology.Grid(4,4))(
  ProcessInitializer { p =>
    val app = new BcastApp(p)
    val traverse = new ParallelTraversal(p)
    app --> traverse
  }
)

