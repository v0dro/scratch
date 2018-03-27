a = [[56, 57, 58, 59, 60, 61, 62, 63];
    [48, 49, 50, 51, 52, 53, 54, 55];
    [40, 41, 42, 43, 44, 45, 46, 47];
    [32, 33, 34, 35, 36, 37, 38, 39];
    [24, 25, 26, 27, 28, 29, 30, 31];
    [16, 17, 18, 19, 20, 21, 22, 23];
    [8, 9, 10, 11, 12, 13, 14, 15];
    [0, 1, 2, 3, 4, 5, 6, 7]];
l = zeros(8); u = zeros(8);
blocksize = 2;
A = mat2cell(a, [blocksize, blocksize, blocksize, blocksize], [blocksize, blocksize, blocksize, blocksize]);
L = mat2cell(l, [blocksize, blocksize, blocksize, blocksize], [blocksize, blocksize, blocksize, blocksize]);
U = mat2cell(u, [blocksize, blocksize, blocksize, blocksize], [blocksize, blocksize, blocksize, blocksize]);
N = 4;
for k = 1:N
    diagA = A{k,k};
    [diagL, diagU] = lu(diagA);
    
    L{k,k} = diagL;
    U{k,k} = diagU;
    
    for i = k+1:N
       L{i,k} = A{i,k}/diagU;
       U{k,i} = diagL\A{k,i};
    end
    
    for j = k+1:N
        for i = k+1:N
            A{i,j} = A{i,j} - L{i,k}*U{k,j};
        end
    end
end

cell2mat(L)
cell2mat(U)
lu(a)

A = mat2cell(a, [blocksize, blocksize, blocksize, blocksize], [blocksize, blocksize, blocksize, blocksize]);
for k = 1:N
    diagA = A{k,k};
    [diagL, diagU] = lu(diagA);
    A{k,k} = lu(diagA);
    
    for i = k+1:N
       A{i,k} = A{i,k}/diagU;
       A{k,i} = diagL\A{k,i};
    end
    
    for j = k+1:N
        for i = k+1:N
            A{i,j} = A{i,j} - A{i,k}*A{k,j};
        end
    end
end

cell2mat(A)

