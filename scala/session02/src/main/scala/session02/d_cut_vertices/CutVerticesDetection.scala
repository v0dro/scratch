package session02.d_cut_vertices

import neko._

class CutVerticesDetection(p: ProcessConfig) extends ActiveProtocol(p, "cutvertices")
{
  import CutVerticesDetection.Msg

  listenTo(classOf[Msg])
  def run() {
    var isCutVertex = false
    var round = 0

    // TODO: code comes here

    if (isCutVertex)
      println(s"${me.name} IS a cut vertex")
    else
      println(s"${me.name} is not a cut vertex")
  }
}

object CutVerticesDetection
{
  case class Msg(from: PID, to: PID, round: Int, new_i: Set[PID])
    extends UnicastMessage
}

object CutVerticesMain extends Main(
    topology.Ring(0 to 5)
      union topology.Ring(5 to 10)
      union topology.Clique(10 to 15)
  )( ProcessInitializer { p => new CutVerticesDetection(p) })
