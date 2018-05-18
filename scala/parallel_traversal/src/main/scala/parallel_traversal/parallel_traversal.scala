package parallel_traversal.parallel_traversal

import neko._

object ParallelTraversal {

}

class ParallelTraversal(c: ProcessConfig) extends ReactiveProtocol(c, "parallel traversal")
{
  import ParallelTraversal._

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

