package parallel_bcast.parallel_bcast

import neko._

object ParallelBcast
{
  // define case classes
  case class Go(from: PID, to: PID, payload: List[Double]) extends UnicastMessage
  case class Back(from: PID, to: PID, payload: List[Double]) extends UnicastMessage
  case class Start(from: PID, to: PID) extends UnicastMessage
}

class ParallelTraversal(c: ProcessConfig) extends ReactiveProtocol(c, "bcast ccast")
{
  import ParallelBcast._

  private var parent = PID(-1) // set parent to PID = -1 so its non-existent for now
  private var children = Nil // init children list
  private var expected_msgs = neighbors.size
  private var value_set = Nil

  def onSend = {
    case _ =>
      println("PID is:" + me)
      println("expected:" + expected_msgs)
  }

  listenTo(classOf[Go])
  listenTo(classOf[Back])
  listenTo(classOf[Start])
  def onReceive = {
    case Start(_,_) =>
      // set parent to itself to signify root node.
      parent = me
      val l = 1.0 :: Nil

      for (x <- neighbors.toIterator) {
        SEND(Go(me, x, l))
      }
    case Go(from, to, msg) =>
      if (parent == PID(-1)) {
        // set the parent to what ever process among the neighbors
        //   sends a message first.
        parent = from
        // set children 
        children = Nil
        // indicates that a msg has been received from the parent of
        //   this process.
        expected_msgs -= 1

        // if no more messages to be received.
        if (expected_msgs == 0) {
          SEND(Back(me, parent, msg))
        }
        else {
          for (x <- neighbors.toIterator) {
            SEND(Go(me, x, msg))
          }
        }
      }
      else {
        val msg = Nil
        SEND(Back(me, from, msg))
      }
    case Back(from, to, msg) =>
      // Successfully received back a message so decrement expected_msgs counter.
      expected_msgs -= 1
      if (expected_msgs == 0) {
        val new_msg = 1.0 :: msg
        if (parent != me) { // if this is not the root node
          SEND(Back(me, parent, new_msg))
        }
        else { // if this is the root node
          println("computed value: " + compute_function(new_msg))
        }
      }
    case _ =>
      println("nothing")
  }

  def compute_function(msg: List[Double]) : Double = {
    var sum = 0.0
    for (x <- msg.toIterator) {
      sum += x
    }

    return sum
  }
}

object ParallelBcastMain extends Main(topology.Grid(4,4))(
  ProcessInitializer { p =>
    val app = new BcastApp(p)
    val traverse = new ParallelTraversal(p)
    app --> traverse
  }
)

