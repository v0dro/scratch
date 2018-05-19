package parallel_traversal.parallel_traversal

import neko._
import neko.util.TraversalClient

object ParallelTraversal {
  case class Token(from: PID, to: PID) extends UnicastMessage
}

class ParallelTraversal(c: ProcessConfig) extends ReactiveProtocol(c, "parallel traversal")
{
  import ParallelTraversal._
  import TraversalClient._
  import Tarry._

  private var children = List.empty[PID]
  private var parent : Option[PID] = None

  def 

  def onReceive = PartialFunction.empty[Event,Unit]

  def onSend = PartialFunction.empty[Event,Unit]

}

object ParallelTraversalMain extends Main(topology.Clique(10))(
  ProcessInitializer { p =>
    val app = new AppTraversal(p)
    val traverse = new ParallelTraversal(p)
    app --> traverse
  }
)

