module minidraw

libp = "../minidraw/libminidraw.so"

function version()
    unsafe_string(@ccall libp.version()::Cstring)
end

function initDrawBoard(x,y)
    @ccall libp.initDrawBoard(x::Cint,y::Cint)::Cvoid
end

function setColor(r,g,b)
    @ccall libp.setColor(r::Cint,g::Cint,b::Cint)::Cvoid
end

function lineTo(x,y)
    @ccall libp.lineTo(x::Cint,y::Cint)::Cvoid
end

function moveTo(x,y)
    @ccall libp.moveTo(x::Cint,y::Cint)::Cvoid
end

function stroke()
    @ccall libp.stroke()::Cvoid
end

function savePng(name)
    @ccall libp.savePng(name::Cstring)::Cvoid
end

end