package session01.d_traversal

import neko._
import neko.util.TraversalClient


class Tarry (p:ProcessConfig)
  extends ReactiveProtocol(p, "Tarry")
{
  import Tarry._
  import TraversalClient._

  private def isRoot = parent.contains(me)
  private var parent : Option[PID] = None
  private var children = List.empty[PID]

  private def visitNextChild() {
    children match {
      case next :: tail    =>   // TODO: Rule 1
      case Nil if ! isRoot =>   // TODO: Rule 2
      case Nil if isRoot   => DELIVER (Done)                            // End
    }
  }

  def onSend = {
    case Initiate =>
      parent   = Some(me)
      children = neighbors.toList
      DELIVER (Visit)
      visitNextChild()
  }

  listenTo(classOf[Token])
  def onReceive = {
    case Token(from, _) if parent.isEmpty =>
      parent   = Some(from)
      children = (neighbors-from).toList
      DELIVER (Visit)
      visitNextChild()
    case Token(_, _) => visitNextChild()
  }
}


object Tarry
{
  case class Token(from: PID, to: PID)
    extends UnicastMessage
}


object TarryMain
  extends Main(topology.Grid(4, 4), withTrace = true)(
    ProcessInitializer { p =>
      val app   = new AppTraversal(p)
      val tarry = new Tarry(p)
      app --> tarry
    }
  )
