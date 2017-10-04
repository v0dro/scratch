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


