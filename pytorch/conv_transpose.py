import torch

class _NewEmptyTensorOp(torch.autograd.Function):
    @staticmethod
    def forward(ctx, x, new_shape):
        ctx.shape = x.shape
        print("NewEmptyTensorOp.forward")
        return x.new_empty(new_shape).clamp(min=0)

    @staticmethod
    def backward(ctx, grad):
        shape = ctx.shape
        # call forward again with grad and shape as parameters.
        print("NewEmptyTensorOp.backward ", shape)
        return _NewEmptyTensorOp.apply(grad, shape), None

class ConvTranspos1d(torch.nn.ConvTranspose1d):
    def forward(self, x):
        if x.numel() > 0:
            print("x numel: " , x.numel())
            return super(ConvTranspos1d, self).forward(x)
        # get output shape

        print("hello world...")
        output_shape = [
            (i - 1) * d - 2 * p + (di * (k - 1) + 1) + op
            for i, p, di, k, d, op in zip(
                x.shape[-2:],
                self.padding,
                self.dilation,
                self.kernel_size,
                self.stride,
                self.output_padding,
            )
        ]
        print("first: ", output_shape)
        output_shape = [x.shape[0], self.bias.shape[0]] + output_shape
        print("second: ", output_shape)
        return _NewEmptyTensorOp.apply(x, output_shape)

    @staticmethod
    def backward(ctx, grad):
        shape = ctx.shape
        # call forward again with grad and shape as parameters.
        print("backward shape: " , shape)
        return _NewEmptyTensorOp.apply(grad, shape), None

y = torch.ones(1,1,1)
d1 = ConvTranspos1d(1, 1, 3)
d1.weight.data = torch.ones(1,1,3)
b = y.new_empty([0,1,1])
q = d1(b)
print(q)
# q *= 2
# q.backward(torch.ones(q.shape))

