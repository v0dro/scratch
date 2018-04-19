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
 * Date: 20/07/15
 * Time: 22:35
 *
 */
package session02.c_leader_anon

import neko._


/**
 * Randomized leader election in anonymous networks in constant expected number of rounds.
 *
 * @param p process configuration
 */
class AnonLeader(p: ProcessConfig) extends ReactiveProtocol(p, "anonymous leader")
{
  import AnonLeader._
  import protocol.FastAnonymousRounds._

  val rand  = new scala.util.Random()
  var myBit = rand.nextBoolean() // FIXME: improve random selection

  listenTo(InitRound.getClass)
  listenTo(classOf[StartRound])

  def onReceive =
  {
    case InitRound =>
      SEND(Draw(myBit))

    case StartRound(round, info) =>
      val roundData = info.map(_.asInstanceOf[Draw])

      displayRound(round, roundData)

      val countTrue = roundData.count(_.bit == true)

      if (countTrue == 1) {
        if (myBit) {
          /* LEADER */
          println (s"${me.name } LEADER !!!!! (round = $round)")
        }
        SEND(Done)
      }
      else {
        myBit = rand.nextBoolean()  // FIXME: improve random selection
        SEND(Draw(myBit))
      }
  }

  def onSend = PartialFunction.empty


  private def displayRound(round: Int, roundData: Seq[Draw]): Unit =
    if (me == PID(0)) { // Display information on the round
      val vect   = roundData.map(d => if (d.bit) "O" else "_").mkString(" ")
      println(s"$round : $vect")
    }
}


object AnonLeader
{
  case class Draw(bit: Boolean) extends protocol.FastAnonymousRounds.Anonymous
}


object AnonLeaderMain
  extends Main(topology.Clique(2000))(
    ProcessInitializer { p =>
      val leader = new AnonLeader(p)
      val anon   = new protocol.FastAnonymousRounds(p)
      leader --> anon
    }
  )
