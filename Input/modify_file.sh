#!/bin/bash
input_filename=$1
query_filename=$2
output_queryfile=new_query_b
output_inputfile=new_input_b
num=1
while read -r line
do
	name="$line"
	echo "item_id$num $line" >> "$output_inputfile"
	let num=num+1
done < "$input_filename"
num=1
while read -r line
do
	name="$line"
	echo "item_idS$num $line" >> "$output_queryfile"
	let num=num+1
done < "$query_filename"

	

