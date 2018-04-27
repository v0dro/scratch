/**
 *
 * Copyright 2015 Xavier Defago
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Created by IntelliJ IDEA.
 * User: defago
 * Date: 24/06/15
 * Time: 13:07
 *
 */
package session02.a_leader_ring

import neko._

class ChangRoberts(c: ProcessConfig) extends ReactiveProtocol(c, "election")
{
  import ChangRoberts._
  import neko.util.LeaderElectionClient._

  private val next = me.map (i => (i+1) % N )
  private var color : StateColor = Red

  def onSend = {
    case Candidate =>
      if (color == Red) SEND (Election(me, next, me))
  }

  listenTo(classOf[Election])
  listenTo(classOf[AnnounceLeader])

  def onReceive = {
    case Election(_,_,candidate) if candidate > me =>
      color = Black
      SEND( Election(me, next, candidate) )

    case Election(_,_,`me`) => SEND( AnnounceLeader(me,ALL,me) )

    case Election(_,_,candidate) if candidate < me => /* DROP token */

    case AnnounceLeader(_,_,leader) => DELIVER( Elected(Some(leader)) )

    case _ => /* otherwise: IGNORE */
  }
}


object ChangRoberts
{
  case class Election(from: PID, to: PID, candidate: PID) extends UnicastMessage
  case class AnnounceLeader(from: PID, to: Set[PID], leader: PID) extends MulticastMessage

  sealed abstract class StateColor
  case object Red   extends StateColor
  case object Black extends StateColor
}

object ChangRobertsMain
  extends Main(topology.Clique(20))(
    ProcessInitializer { p=>
      val app    = new LeaderApp(p)
      val leader = new ChangRoberts(p)
      app --> leader
    }
  )
