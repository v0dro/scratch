type 'a binary_tree =
  Empty
| Node of 'a binary_tree * 'a  * 'a binary_tree

let rec values t = 
  match t with
    | Empty -> []
    | Node(left, x,right) -> values left @ [x] @ values right;;

values ()



let is_binary_search_tree tree =
  let rec aux test t =
    match t with
      | Empty -> true
      | Node(y, left, right) ->
        test y &&
        aux (function x -> x < y) left &&
        aux (function x -> x > y) right in 
  aux (function _-> true) t;;