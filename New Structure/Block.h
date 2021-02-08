class Block{
    public:
        // Default constructor, creates a block with ID 0 (empty/air)
        Block();
        // Create a block
        Block(int blockID);
        Block(int blockDepth, Block frontBlock, Block leftBlock, Block topBack);
        const int blockID;
    private:
        int xPos;
        int yPos;
        int zPos;
};