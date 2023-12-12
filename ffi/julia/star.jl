include("./minidraw.jl")

println(minidraw.version());

L = 100

r = L
dx=round(r*cos(deg2rad(30)))
dy=round(r*sin(deg2rad(30)))

minidraw.initDrawBoard(2*dx,2*L);
minidraw.setColor(0,125,125)

minidraw.moveTo(dx,0)
minidraw.lineTo(2*dx,L+dy)
minidraw.lineTo(0,L+dy)
minidraw.lineTo(dx,0)
minidraw.stroke()

minidraw.moveTo(dx,2*L)
minidraw.lineTo(0,L-dy)
minidraw.lineTo(2*dx,L-dy)
minidraw.lineTo(dx,2*L)
minidraw.stroke()

minidraw.savePng("star.png");