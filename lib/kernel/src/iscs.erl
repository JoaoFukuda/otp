-module(iscs).
-export([checktype/1, insert/1, lookup/1, update/2, update_counter/2]).

checktype(_) ->
	erlang:nif_error(undef).

insert(_) ->
	erlang:nif_error(undef).

lookup(_) ->
	erlang:nif_error(undef).

update(_, _) ->
	erlang:nif_error(undef).

update_counter(_, _) ->
	erlang:nif_error(undef).
