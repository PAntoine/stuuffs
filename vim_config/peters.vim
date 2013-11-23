"                Peter's Generic VimScript
"
" This file is the basic vim script file for my projects.
" It has the default file command layouts.
"
" Also, it has allows for the root directory of a project to have
" a .banner and .copyright file. If either of these files are
" present then the content is incorporated into the file header.
"
" This file also has various simple commands that make coding
" a little easier.
"
"           Copyright (c) 2010 & 2011 Peter Antoine
"             Released under the Artistic Licence
"
" Author: Peter Antoine 
" Date  : 16th Feb 2010

if !exists("s:homenet_def_vim")
	let s:homenet_def_vim = 1

let s:top_line = "---------------------------------------------------------------------------------"
let s:c_line = "--------------------------------------------------------------------------------*"
let s:number_lines = 0
let s:insert_line = 0
let s:insert_row = 0
				
function HOMENET_file_header(start_line,line,line_1, line_2,term)
		if filereadable(".banner")
			let top_banner = readfile(".banner")
		else
			let top_banner = []
		endif

		if filereadable(".copyright")
			let copyright = readfile(".copyright")
			for x in copyright
			 let copyright[x] = substitute(copyright[x],"YYYY",strftime("%Y"),"")
			endfor
		else
			let copyright = [
					\"                     Copyright (c) " . strftime("%Y") . " Peter Antoine",
                    \"                           All rights Reserved.",
                    \"                      Released Under the MIT Licence" ]
		endif

		let middle = [
					\"    file: " . expand("%:t:r"),
					\"    desc: ",
					\"",
					\"  author: " . $USERNAME,
					\"    date: " . strftime("%d/%m/%Y")]

		let header = map(top_banner + middle + [a:line] + copyright + [a:line . a:term], 'a:line_2 . v:val' )

		call setline(a:start_line,[a:line_1 . a:line] + header)
		let s:number_lines = len(header) + 1 + a:start_line
		let s:insert_line = len(top_banner) + 2 + a:start_line
		let s:insert_row = 12

		unlet header
		unlet middle
		unlet copyright
		unlet top_banner
endfunction

function HOMENET_put_src_file_header()
	let line_1 = "/*"
	let line_2 = " *"
	call HOMENET_file_header(1,s:c_line,line_1,line_2,"/")
	call setpos(".",[0,s:insert_line,s:insert_row,-1])
	unlet line_1
	unlet line_2
endfunction

function HOMENET_put_hdr_file_header()
	let line_1 = "/*"
	let line_2 = " *"
	let header_stuff = [
					\"",
					\"#ifndef __"  . toupper(expand("%:t:r")) . "_H__",
					\"#define __"  . toupper(expand("%:t:r")) . "_H__",
					\"",
					\"#endif",
					\""]

	call HOMENET_file_header(1,s:c_line,line_1,line_2,"/")

	call setline(s:number_lines,header_stuff)
	call setpos(".",[0,s:insert_line,s:insert_row,-1])

	unlet line_1
	unlet line_2
	unlet header_stuff
endfunction

function HOMENET_put_xml_file_header()
	let header_stuff = [
						\'<?xml version="1.0" encoding="utf-8"?>',
						\"<!--",
						\"/*"]
						
	call setline(1,header_stuff)
	call HOMENET_file_header(4,"=================================================================================","**","**","")
	let footer_stuff = [
						\"*/",
						\"-->"]
	call setline(s:number_lines,footer_stuff)
	call setpos(".",[0,s:insert_line,s:insert_row,-1])
	unlet header_stuff
	unlet footer_stuff
endfunction

function HOMENET_put_mak_file_header()
	call HOMENET_file_header(1,s:top_line,"#","#","")
	call setpos(".",[0,s:insert_line,s:insert_row,-1])
endfunction

function HOMENET_put_bash_file_header()
	call setline(1,"#!/bin/bash")
	call HOMENET_file_header(2,s:top_line,"#","#","")
	call setpos(".",[0,s:insert_line,s:insert_row,-1])
endfunction

function HOMENET_put_py_file_header()
	call setline(1,"#!/usr/bin/env python")
	call setline(2,"# -*- coding: utf-8 -*-")
	call HOMENET_file_header(3,s:top_line,"#","#","")
	call setline(line('$')+1,"")
	call setline(line('$')+1,"# vim: ts=4 sw=4 noexpandtab nocin ai")	" should add to the end
	call setpos(".",[0,s:insert_line,s:insert_row,-1])
endfunction

function HOMENET_put_java_file_header()
	call HOMENET_file_header(1,s:top_line,"//","//","")
	call setpos(".",[0,s:insert_line,s:insert_row,-1])
endfunction

function HOMENET_put_vhdl_file_header()
	call HOMENET_file_header(1,s:top_line,"--","--","")
	call setpos(".",[0,s:insert_line,s:insert_row,-1])
endfunction

function HOMENET_put_vim_file_header()
	call HOMENET_file_header(1,s:top_line,"\" ","\" ","")
	call setpos(".",[0,s:insert_line,s:insert_row,-1])
endfunction

function HOMENET_add_frame()
	call append(line("."),"")
	call append(line(".")," *--------------------------------------------------------------------------------*/")
	call append(line(".")," * ")
	call append(line("."),"/*--------------------------------------------------------------------------------*")
	call setpos(".",[0,line(".")+2,3,-1])
endfunction

function HOMENET_add_api()
	let function_name = input("Function Name: ")

	if ( function_name != '')
		let return_type = input("Return Type: ","void")

		let cur_pos = line(".")
		call HOMENET_write_api(function_name,return_type,"")
		call setpos(".",[0,cur_pos+3,17,-1])

		unlet return_type
	endif

	unlet function_name
endfunction	


function HOMENET_add_function()
	let function_name = input("Function Name: ")

	if ( function_name != '')
		let return_type = input("Return Type: ","void")

		let cur_pos = line(".")
		call HOMENET_write_function(function_name,return_type,"")
		call setpos(".",[0,cur_pos+3,10,-1])

		unlet return_type
	endif

	unlet function_name
endfunction	

function HOMENET_write_function(function_name,return_type,parameter_list)

	if (a:function_name != '')
		let size = 9
		let input = ["/*----- FUNCTION -----------------------------------------------------------------*",
					\" *  name: " . a:function_name,
					\" *  desc: ",
					\" *--------------------------------------------------------------------------------*/",
					\a:return_type . "	" . a:function_name . "(" . a:parameter_list . ")",
					\"{"]

		if (a:return_type != 'void')
			call extend(input,["	" . a:return_type . " result;","","	return result;","}",""])
			let size += 4
		else
			call extend(input,["}",""])
		endif

		call append(line("."),input)
	endif
endfunction

function HOMENET_write_api(function_name,return_type,parameter_list)

	if (a:function_name != '')
		let size = 9
		let input = ["/**---- API ----------------------------------------------------------------------*",
					\" *  @api         " . a:function_name,
					\" *  @description ",
					\" *  @action ",
					\" *  @parameter ",
					\" *  @returns ",
					\" *--------------------------------------------------------------------------------*/",
					\a:return_type . "	" . a:function_name . "(" . a:parameter_list . ")",
					\"{"]

		if (a:return_type != 'void')
			call extend(input,["	" . a:return_type . " result;","","	return result;","}",""])
			let size += 4
		else
			call extend(input,["}",""])
		endif

		call append(line("."),input)
	endif
endfunction


function HOMENET_make_function() range

	let lines = getline(a:firstline,a:lastline)
	let lines = reverse(lines)

	execute ":" . a:firstline . "," . a:lastline . "d"

	for x in lines
		let return_type = substitute(x,"\\(unsigned\\s\\S\\+\\|\\S\\+\\)\\s.\\+$","\\1","")
		let function_name = substitute(x,"^\\(unsigned\\s\\S\\+\\|\\S\\+\\)\\s\\+\\([a-zA-Z0-9_]\\+\\)(.\\+$","\\2","")
		let parameters = substitute(x,"^.\\+(\\(.\\+\\)).\\+$","\\1","")
		call HOMENET_write_function(function_name,return_type,parameters)
	endfor

	unlet lines
	unlet return_type
	unlet function_name
	unlet parameters
endfunction

" Set the autocommands for the filetypes allowed
au BufNewFile *.h call HOMENET_put_hdr_file_header()
au BufNewFile *.c call HOMENET_put_src_file_header()
au BufNewFile *.js call HOMENET_put_src_file_header()
au BufNewFile *.m call HOMENET_put_src_file_header()
au BufNewFile *.vim call HOMENET_put_vim_file_header()
au BufNewFile *.cpp call HOMENET_put_src_file_header()
au BufNewFile *.vhdl call HOMENET_put_vhdl_file_header()
au BufNewFile *.java call HOMENET_put_java_file_header()
au BufNewFile *.aidl call HOMENET_put_java_file_header()
au BufNewFile *.sh call HOMENET_put_bash_file_header()
au BufNewFile *.mak call HOMENET_put_mak_file_header()
au BufNewFile *.xml call HOMENET_put_xml_file_header()
au BufNewFile *.py call HOMENET_put_py_file_header()
au BufNewFile makefile call HOMENET_put_mak_file_header()

" Set the mappings for the other stuff
map <C-\>a :call HOMENET_add_api()<cr>
map <C-\>f :call HOMENET_add_function()<cr>
map <C-\>c :call HOMENET_add_frame()<cr>
map <C-\>m :call HOMENET_make_function()<cr>

endif
