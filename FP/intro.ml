(*
Name: Sameer Deshmukh
Student ID: 17M38101
 *)

let pi = 4.0 *. atan 1.0;;

let circle_area radius =
  pi *. radius *. radius;;

let radius_of_circle area =        
  sqrt (area /. pi);;

let rec circle_areas r_list =
  match r_list with
    [] -> []
  | hd :: tl -> circle_area hd :: circle_areas tl;;

type iorf = Int of int | Float of float | Error;;

let div_num num1 num2 =
  match (num1, num2) with
    (Int n1, Int n2) -> Int(n1 / n2)
  | (Int n1, Float f2) -> Float(float n1 /. f2)
  | (Float num1, Int num2) -> Float(num1 /. float num2)
  | (Float num1, Float num2) -> Float(num1 /. num2)
  | (_, Error) -> Error
  | (Error, _) -> Error;;
                                     
type 'a binary_tree =
  Empty
| Node of 'a * 'a binary_tree * 'a binary_tree;;


let rec find x tree =
  match tree with
    Empty -> false
  | Node(y, left, right) ->
     if x = y then true else
       if x < y then find y left else find y right;;
