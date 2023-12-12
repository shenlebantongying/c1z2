struct sab
    a::Int32
    b::Int32
end

a = sab(200, 100)

# note that @ccall convert a's type to reference automatically
res = @ccall "../ftype/libpointer-types.so".plus_sab(a::Ref{sab})::Cint

println(res)