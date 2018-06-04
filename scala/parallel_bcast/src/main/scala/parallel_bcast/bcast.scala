import neko.PID
import neko.Main
import neko.ProcessInitializer
import neko.topology
import neko.ProcessConfig
import neko.ReactiveProtocol
import neko.UnicastMessage
import neko.MulticastMessage
import collection.mutable.Map
import collection.mutable.ArrayBuffer

class ParallelBcast (p:ProcessConfig)
  extends ReactiveProtocol(p, "ParallelBcast")
{
  import ParallelBcast.Go
  import ParallelBcast.Back
  import ParallelBcast.compute

  private var parent = ArrayBuffer[PID]()
  private var pid_to_index = Map[PID, Int]()
  private var children = ArrayBuffer[Set[PID]]()
  private var expected_msg = ArrayBuffer[Int]()
  private var val_set = ArrayBuffer[Set[List[Int]]]()

  def onSend = {
    case initiate =>
      pid_to_index += (me -> parent.size)
      parent += me
      children += Set.empty[PID]
      val_set += Set.empty[List[Int]]
      expected_msg += neighbors.toList.size
      SEND(Go(me, neighbors, me))
  }

  listenTo(classOf[Go])
  listenTo(classOf[Back])
  def onReceive = {
    case Go(from, _, k) =>
      println(s"Received GO from ${from} at me: ${me}")
      if (!pid_to_index.contains(k)) {
        pid_to_index += (k -> parent.size)
        parent += from
        children += Set.empty[PID]
        val_set += Set(List(me.value, k.value))
        expected_msg += (neighbors).toList.size - 1
        if (expected_msg(pid_to_index(k)) == 0) {
          SEND(Back(me, from, val_set(pid_to_index(k)), k))
        } else {
          SEND(Go(me, neighbors - from, k))
        }
      } else {
        SEND(Back(me, from, Set.empty[List[Int]], k))
      }
    case Back(from, _, val_set_, k) =>
      println(s"Received BACK from ${from} at me: ${me}")
      expected_msg(pid_to_index(k)) -= 1
      if (!val_set.isEmpty) {
        children(pid_to_index(k)) = children(pid_to_index(k)) + from
        val_set(pid_to_index(k)) =  val_set(pid_to_index(k)) | val_set_
      }
      if (expected_msg(pid_to_index(k)) == 0) {
        val_set(pid_to_index(k)) = val_set(pid_to_index(k))
        if (parent(pid_to_index(k)) != me) {
          SEND(Back(me, parent(pid_to_index(k)), val_set(pid_to_index(k)), k))
        } else {
          val calc_root: Int = compute(val_set(pid_to_index(me)), me)
          val check: Boolean = (calc_root == me.value)
          println(s"Root: ${me.value}. Result : ${calc_root}")
        }
      }
  }
}

object ParallelBcast
{
  case class Go(from: PID, to: Set[PID], val k: PID)
    extends MulticastMessage
  case class Back(from: PID, to: PID, val val_set: Set[List[Int]], val k: PID)
    extends UnicastMessage
  def compute (val_set: Set[List[Int]], k: PID) : Int = {
    var sum: Int = 0
    val_set.foreach((set: List[Int]) => sum += set(0) * (set(1) + 1))
    sum /= (k.value + 1)
    var root_id: Int = (val_set.size * (val_set.size + 1))/2 - sum
    return root_id
  }
}

object MainParallelBcast
  extends Main(topology.Grid(8, 8)) (
    ProcessInitializer { p =>
      val app0 = new BcastApp(p, PID(0))
      val app1 = new BcastApp(p, PID(5))
      val app2 = new BcastApp(p, PID(10))
      val app3 = new BcastApp(p, PID(15))
      val app4 = new BcastApp(p, PID(20))
      val bfs = new ParallelBcast(p)
      app0 --> bfs
      app1 --> bfs
      app2 --> bfs
      app3 --> bfs
      app4 --> bfs
    }
  )
