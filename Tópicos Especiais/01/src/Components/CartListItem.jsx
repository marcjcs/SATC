import InputCounter from '../Components/InputCounter';

function CartListItem({
  title,
  description,
  image,
  price,
  quantity,
  onUpdateQuantity,
  onRemoveItem,
  disabled,
}) {
  return (
    <div
      className="py-6"
      style={disabled ? { pointerEvents: 'none', opacity: 0.6 } : {}}
    >
      <div className="flex bg-white shadow-lg rounded-lg overflow-hidden">
        <div
          className="w-1/3 bg-cover"
          style={{ backgroundImage: `url(${image})` }}
        ></div>
        <div className="w-2/3 p-4">
          <div className="flex flex-row">
            <h1 className="text-gray-900 font-bold text-2xl">{title}</h1>
            <button
              onClick={() => onRemoveItem()}
              className="px-3 py-2 bg-gray-800 text-white text-xs font-bold uppercase rounded"
            >
              Remove
            </button>
          </div>
          {disabled && (
            <p className="text-red-700 font-bold">Item already bought</p>
          )}
          <p className="mt-2 text-gray-600 text-sm">{description}</p>
          <div className="flex item-center justify-between mt-3">
            <h1 className="text-gray-700 font-bold text-xl">
              ${price.toFixed(2)}
            </h1>
            <InputCounter
              value={quantity}
              onChange={(value) => {
                onUpdateQuantity(value);
              }}
            />
          </div>
        </div>
      </div>
    </div>
  );
}

export default CartListItem;
