
function FFT_prepare(f)
	for _i in 1:f.len
		p=0
		i=_i-1
		for j in 1:f.bits
			p=(p*2)+ ( (i%2)==1 ? 1 : 0 )
			i=fld(i,2)
		end
		#println(_i-1," ",p)
		f.exchangeMap[_i]=p+1
	end
	for i in 1:fld(f.len,2)
		ang=-2*pi*(i-1)/f.len
		f.e[i]=cos(ang)+sin(ang)*im
	end
	return f
end

struct FFT
	bits::Integer
	len::Integer
	exchangeMap
	e
	FFT(bits)=FFT_prepare(new(	bits,
					2^bits,
					zeros(Int,2^bits),
					zeros(Complex,2^(bits-1))))
end

function (fft::FFT)(f)
	F=zeros(Complex,2^(fft.bits) )
	for i in 1:fft.len
		F[fft.exchangeMap[i]]=f[i]
	end

	for i in 0:(fft.bits-1)
		unitSize_2=2^i
		unitSize=2*unitSize_2
		unitNum=2^(fft.bits-i-1)
		
		#=
		for j in 1:unitSize:
			coef=fft.e[(j-1)*unitNum+1]
			for k in 0:(unitNum-1):
				pPre= unitSize*k+j
				pPost=pPre+unitSize_2
				pre=F[pPre]
				post=F[pPost]
				F[pPre]=pre+coef*post
				F[pPost]=pre-coef*post
		=#
		for k in 0:(unitNum-1)
			offset=unitSize*k+1
			for j in 0:(unitSize_2-1)
				coef=fft.e[j*unitNum+1]
				pPre=offset+j
				pPost=pPre+unitSize_2
				pre=F[pPre]
				post=F[pPost]*coef
				#println([pre,post,coef])
				F[pPre]=pre+post
				F[pPost]=pre-post
			end
		end
	end
	return F./fft.len
end

f2=FFT(2)
f2=FFT(2)
println(f2)
println(f2([0,1,2,3]))

println()
f3=FFT(3)
println(f3)
println(f3(0:7))
F10=FFT(10)
for i in 1:(10)
	F10(rand(1024))
end
