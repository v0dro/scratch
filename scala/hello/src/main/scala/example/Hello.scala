package session01.a_hello

import neko._

class Hello(p: ProcessConfig) extends ActiveProtocol(p, "hello")
{
  def run() {
    println(s"Process ${me.name} says: 'Hello Neko World!'")
  }
}

object HelloNeko
  extends Main(topology.Clique(100))(
    ProcessInitializer { p => new Hello(p) }
  )
