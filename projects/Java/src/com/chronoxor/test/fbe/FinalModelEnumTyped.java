//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: test.fbe
// FBE version: 1.10.0.0
//------------------------------------------------------------------------------

package com.chronoxor.test.fbe;

// Fast Binary Encoding EnumTyped final model
public final class FinalModelEnumTyped extends com.chronoxor.fbe.FinalModel
{
    public FinalModelEnumTyped(com.chronoxor.fbe.Buffer buffer, long offset) { super(buffer, offset); }

    // Get the allocation size
    public long fbeAllocationSize(com.chronoxor.test.EnumTyped value) { return fbeSize(); }

    // Get the final size
    @Override
    public long fbeSize() { return 1; }

    // Check if the value is valid
    @Override
    public long verify()
    {
        if ((_buffer.getOffset() + fbeOffset() + fbeSize()) > _buffer.getSize())
            return Long.MAX_VALUE;

        return fbeSize();
    }

    // Get the value
    public com.chronoxor.test.EnumTyped get(com.chronoxor.fbe.Size size)
    {
        if ((_buffer.getOffset() + fbeOffset() + fbeSize()) > _buffer.getSize())
            return new com.chronoxor.test.EnumTyped();

        size.value = fbeSize();
        return new com.chronoxor.test.EnumTyped(readInt8(fbeOffset()));
    }

    // Set the value
    public long set(com.chronoxor.test.EnumTyped value)
    {
        assert ((_buffer.getOffset() + fbeOffset() + fbeSize()) <= _buffer.getSize()) : "Model is broken!";
        if ((_buffer.getOffset() + fbeOffset() + fbeSize()) > _buffer.getSize())
            return 0;

        write(fbeOffset(), value.getRaw());
        return fbeSize();
    }
}